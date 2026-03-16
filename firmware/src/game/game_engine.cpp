#include "game/game_engine.h"

#include <Arduino.h>

#include "config/game_balance.h"
#include "config/ui_config.h"
#include "game/rules.h"
#include "utils/logger.h"

namespace xenovent::game {

GameEngine::GameEngine(storage::StateStorage& storage, ui::Renderer& renderer,
                       input::ButtonManager& buttons, drivers::Adxl345Manager& adxl,
                       drivers::Bh1750Manager& bh1750, output::OutputManager& output,
                       ui::UiFsm& fsm, telemetry::EventBus& bus)
    : storage_(storage),
      renderer_(renderer),
      buttons_(buttons),
      adxl_(adxl),
      bh1750_(bh1750),
      output_(output),
      fsm_(fsm),
      bus_(bus) {}

void GameEngine::begin() {
  bus_.publish({telemetry::EventType::AppStartup, millis(), 0, 0, "startup_begin"});
  utils::info("Engine", "startup sequence begin");
  storage_.begin();

  if (storage_.hasSavedState()) {
    auto loaded = storage_.loadState();
    state_ = loaded.state;
    bus_.publish({telemetry::EventType::StorageLoad, millis(), loaded.loadedFromNvs ? 1 : 0,
                  static_cast<int32_t>(loaded.recoveryStatus), "load_state"});
    if (loaded.migration.status == storage::RecoveryStatus::MigrationApplied) {
      bus_.publish({telemetry::EventType::StorageMigration, millis(),
                    static_cast<int32_t>(loaded.migration.fromVersion),
                    static_cast<int32_t>(loaded.migration.toVersion), loaded.migration.message});
    }
    if (loaded.recoveryStatus != storage::RecoveryStatus::None &&
        loaded.recoveryStatus != storage::RecoveryStatus::MigrationApplied) {
      bus_.publish({telemetry::EventType::StorageRecovery, millis(),
                    static_cast<int32_t>(loaded.recoveryStatus), 0, "storage_recovery"});
    }
    utils::info("Engine", "state loaded from nvs=%d recovery=%d", loaded.loadedFromNvs,
                static_cast<int>(loaded.recoveryStatus));
  } else {
    state_ = storage_.defaultState();
    utils::info("Engine", "default state initialized");
  }

  buttons_.begin();
  adxl_.begin();
  bh1750_.begin();
  output_.begin();
  renderer_.begin();

  flags_ = domain::deriveUiFlags(state_);

  lastFastLoopMs_ = millis();
  lastTickMs_ = lastFastLoopMs_;
  lastAutosaveMs_ = lastFastLoopMs_;
  bus_.publish({telemetry::EventType::AppStartup, millis(), 1, 0, "startup_done"});
  utils::info("Engine", "startup sequence done");
}

void GameEngine::handleInput(const input::InputEvent& event, uint32_t nowMs) {
  bus_.publish({telemetry::EventType::InputInterpreted, nowMs, static_cast<int32_t>(event.type),
                static_cast<int32_t>(event.button), "input"});
  const auto prevScreen = fsm_.model().screen;
  const auto t = fsm_.handleEvent(event, nowMs);
  if (prevScreen != t.model.screen) {
    bus_.publish({telemetry::EventType::UiTransition, nowMs, static_cast<int32_t>(prevScreen),
                  static_cast<int32_t>(t.model.screen), "ui_transition"});
  }

  if (t.illegalTransition) {
    utils::debug("UI", "illegal transition rejected");
    return;
  }

  if (t.triggeredAction != domain::ActionType::None) {
    const auto actionResult = domain::applyAction(state_, t.triggeredAction);
    if (actionResult.accepted) {
      const auto beforeCd = domain::getRemainingCooldown(state_, t.triggeredAction);
      state_ = actionResult.after;
      const auto afterCd = domain::getRemainingCooldown(state_, t.triggeredAction);
      flags_ = domain::deriveUiFlags(state_);
      dirty_ = true;
      ritualResidual_ = (t.triggeredAction == domain::ActionType::Ritual);
      fsm_.setTransientMessage(actionResult.message);
      output_.play(output::FeedbackPattern::Confirm);
      bus_.publish({telemetry::EventType::ActionApplied, nowMs,
                    static_cast<int32_t>(t.triggeredAction), 1, actionResult.message});
      if (beforeCd == 0 && afterCd > 0) {
        bus_.publish({telemetry::EventType::CooldownStarted, nowMs,
                      static_cast<int32_t>(t.triggeredAction), static_cast<int32_t>(afterCd),
                      "cooldown_started"});
      }
      utils::info("Action", "%s accepted msg=%s", domain::toString(t.triggeredAction),
                  actionResult.message);
    } else {
      fsm_.setTransientMessage(actionResult.message);
      output_.play(output::FeedbackPattern::Warning);
      bus_.publish({telemetry::EventType::ActionApplied, nowMs,
                    static_cast<int32_t>(t.triggeredAction), 0, actionResult.message});
      utils::warn("Action", "%s rejected reason=%s", domain::toString(t.triggeredAction),
                  actionResult.message);
    }
  }
}

domain::SensorSnapshot GameEngine::readSensors() {
  domain::SensorSnapshot snapshot;
  snapshot.validAccel = adxl_.read(snapshot.accelMagnitude);
  snapshot.validLight = bh1750_.read(snapshot.lightLux);
  return snapshot;
}

void GameEngine::runTick(uint32_t nowMs) {
  const uint32_t elapsedMs = nowMs - lastTickMs_;
  const uint32_t dtSec = elapsedMs / 1000;
  if (dtSec == 0) return;

  domain::TickContext ctx;
  ctx.dtSeconds = dtSec;
  ctx.sensors = readSensors();
  ctx.ritualResidual = ritualResidual_;

  const auto tickResult = processTick(state_, ctx);

  for (auto action : {domain::ActionType::Feed, domain::ActionType::Suppress,
                      domain::ActionType::Ritual, domain::ActionType::Meditate,
                      domain::ActionType::Sleep}) {
    const auto before = domain::getRemainingCooldown(state_, action);
    const auto after = domain::getRemainingCooldown(tickResult.state, action);
    if (before > 0 && after == 0) {
      bus_.publish({telemetry::EventType::CooldownEnded, nowMs, static_cast<int32_t>(action), 0,
                    "cooldown_ended"});
    }
  }

  state_ = tickResult.state;
  flags_ = tickResult.flags;
  ritualResidual_ = false;

  if (tickResult.inputWasNormalized) {
    bus_.publish({telemetry::EventType::WarningRaised, nowMs, 1, 0, "input_normalized"});
    utils::warn("Tick", "input state required normalization");
  }
  if (!tickResult.outputValid) {
    bus_.publish({telemetry::EventType::WarningRaised, nowMs, 2, 0, "output_invalid"});
    utils::error("Tick", "output state failed validation");
    state_ = domain::buildDefaultState();
    flags_ = domain::deriveUiFlags(state_);
  }

  if (tickResult.death.died) {
    output_.play(output::FeedbackPattern::Death);
    fsm_.setTransientMessage("death-reset");
    bus_.publish({telemetry::EventType::DeathReset, nowMs, 1, 0, tickResult.death.reason});
    utils::warn("Tick", "death reason=%s", tickResult.death.reason);
  } else if (tickResult.mutation.mutated) {
    output_.play(output::FeedbackPattern::Mutation);
    fsm_.setTransientMessage(tickResult.mutation.reason);
    bus_.publish({telemetry::EventType::MutationOccurred, nowMs,
                  static_cast<int32_t>(tickResult.mutation.from),
                  static_cast<int32_t>(tickResult.mutation.to), tickResult.mutation.reason});
    utils::info("Tick", "mutation %s -> %s (%s)", domain::toString(tickResult.mutation.from),
                domain::toString(tickResult.mutation.to), tickResult.mutation.reason);
  }

  bus_.publish({telemetry::EventType::TickProcessed, nowMs, static_cast<int32_t>(dtSec), 0,
                tickResult.summary});

  dirty_ = dirty_ || tickResult.recommendSave;
  lastTickMs_ = nowMs;
}

void GameEngine::maybeAutosave(uint32_t nowMs) {
  if (!dirty_) return;
  if (nowMs - lastAutosaveMs_ < config::balance::kAutosaveIntervalMs) return;

  if (storage_.saveState(state_)) {
    dirty_ = false;
    lastAutosaveMs_ = nowMs;
    bus_.publish({telemetry::EventType::StorageSave, nowMs, 1, 0, "autosave_ok"});
    utils::debug("Storage", "autosave ok");
  } else {
    bus_.publish({telemetry::EventType::StorageSave, nowMs, 0, 0, "autosave_failed"});
    utils::error("Storage", "autosave failed");
  }
}

void GameEngine::update() {
  const uint32_t now = millis();

  input::InputEvent event;
  while (buttons_.pollEvent(event, now)) {
    handleInput(event, now);
  }

  fsm_.tick(now);

  if (now - lastTickMs_ >= config::balance::kTickIntervalMs) {
    runTick(now);
  }

  if (now - lastFastLoopMs_ >= config::ui::kFastLoopMs) {
    ui::RenderModel model;
    model.ui = fsm_.model();
    model.creature = state_;
    model.flags = flags_;
    renderer_.render(model);
    bus_.publish({telemetry::EventType::RenderFrame, now, static_cast<int32_t>(model.ui.screen), 0,
                  "render"});
    lastFastLoopMs_ = now;
  }

  maybeAutosave(now);
}

}  // namespace xenovent::game
