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
                       ui::UiFsm& fsm)
    : storage_(storage),
      renderer_(renderer),
      buttons_(buttons),
      adxl_(adxl),
      bh1750_(bh1750),
      output_(output),
      fsm_(fsm) {}

void GameEngine::begin() {
  utils::info("Engine", "startup sequence begin");
  storage_.begin();

  if (storage_.hasSavedState()) {
    auto loaded = storage_.loadState();
    state_ = loaded.state;
    utils::info("Engine", "state loaded from nvs=%d versionMismatch=%d", loaded.loadedFromNvs,
                loaded.versionMismatch);
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
  utils::info("Engine", "startup sequence done");
}

void GameEngine::handleInput(const input::InputEvent& event, uint32_t nowMs) {
  const auto t = fsm_.handleEvent(event, nowMs);

  if (t.illegalTransition) {
    utils::debug("UI", "illegal transition rejected");
    return;
  }

  if (t.triggeredAction != domain::ActionType::None) {
    const auto actionResult = domain::applyAction(state_, t.triggeredAction);
    if (actionResult.accepted) {
      state_ = actionResult.after;
      flags_ = domain::deriveUiFlags(state_);
      dirty_ = true;
      ritualResidual_ = (t.triggeredAction == domain::ActionType::Ritual);
      fsm_.setTransientMessage(actionResult.message);
      output_.play(output::FeedbackPattern::Confirm);
      utils::info("Action", "%s accepted msg=%s", domain::toString(t.triggeredAction),
                  actionResult.message);
    } else {
      fsm_.setTransientMessage(actionResult.message);
      output_.play(output::FeedbackPattern::Warning);
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
  state_ = tickResult.state;
  flags_ = tickResult.flags;
  ritualResidual_ = false;

  if (tickResult.inputWasNormalized) {
    utils::warn("Tick", "input state required normalization");
  }
  if (!tickResult.outputValid) {
    utils::error("Tick", "output state failed validation");
    state_ = domain::buildDefaultState();
    flags_ = domain::deriveUiFlags(state_);
  }

  if (tickResult.death.died) {
    output_.play(output::FeedbackPattern::Death);
    fsm_.setTransientMessage("death-reset");
    utils::warn("Tick", "death reason=%s", tickResult.death.reason);
  } else if (tickResult.mutation.mutated) {
    output_.play(output::FeedbackPattern::Mutation);
    fsm_.setTransientMessage(tickResult.mutation.reason);
    utils::info("Tick", "mutation %s -> %s (%s)", domain::toString(tickResult.mutation.from),
                domain::toString(tickResult.mutation.to), tickResult.mutation.reason);
  }

  dirty_ = dirty_ || tickResult.recommendSave;
  lastTickMs_ = nowMs;
}

void GameEngine::maybeAutosave(uint32_t nowMs) {
  if (!dirty_) return;
  if (nowMs - lastAutosaveMs_ < config::balance::kAutosaveIntervalMs) return;

  if (storage_.saveState(state_)) {
    dirty_ = false;
    lastAutosaveMs_ = nowMs;
    utils::debug("Storage", "autosave ok");
  } else {
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
    lastFastLoopMs_ = now;
  }

  maybeAutosave(now);
}

}  // namespace xenovent::game
