#include "game/game_engine.h"

#include <Arduino.h>

#include "config/constants.h"

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
  storage_.begin();
  state_ = storage_.loadState();
  buttons_.begin();
  adxl_.begin();
  bh1750_.begin();
  output_.begin();
  renderer_.begin();
  lastFastLoopMs_ = millis();
  lastTickMs_ = lastFastLoopMs_;
  lastAutosaveMs_ = lastFastLoopMs_;
}

void GameEngine::update() {
  const uint32_t now = millis();

  input::InputEvent event;
  while (buttons_.pollEvent(event, now)) {
    handleInput(event);
  }

  if (now - lastTickMs_ >= config::kTickIntervalMs) {
    updateTick(now);
  }

  if (now - lastFastLoopMs_ >= config::kFastLoopMs) {
    renderer_.render(fsm_.currentState(), state_, fsm_.selectedAction());
    lastFastLoopMs_ = now;
  }

  maybeAutosave(now);
}

void GameEngine::handleInput(const input::InputEvent& event) {
  ui::UiTransition transition = fsm_.handleEvent(event);

  if (transition.triggeredAction != domain::ActionType::None) {
    domain::applyAction(state_, transition.triggeredAction);
    output_.notifyAction(transition.triggeredAction);
    dirty_ = true;
  }

  if (transition.forceSuppress) {
    domain::applyAction(state_, domain::ActionType::Suppress);
    output_.notifyAction(domain::ActionType::Suppress);
    dirty_ = true;
  }
}

domain::SensorSnapshot GameEngine::readSensors() {
  domain::SensorSnapshot snapshot;
  snapshot.validAccel = adxl_.read(snapshot.accelMagnitude);
  snapshot.validLight = bh1750_.read(snapshot.lightLux);
  return snapshot;
}

void GameEngine::updateTick(uint32_t nowMs) {
  const uint32_t elapsedMs = nowMs - lastTickMs_;
  const uint32_t dtSec = elapsedMs / 1000;
  if (dtSec == 0) {
    return;
  }

  domain::TickContext ctx;
  ctx.dtSeconds = dtSec;
  ctx.sensors = readSensors();
  processTick(state_, ctx);
  dirty_ = true;
  lastTickMs_ = nowMs;
}

void GameEngine::maybeAutosave(uint32_t nowMs) {
  if (!dirty_) {
    return;
  }
  if (nowMs - lastAutosaveMs_ >= config::kAutosaveIntervalMs) {
    storage_.saveState(state_);
    dirty_ = false;
    lastAutosaveMs_ = nowMs;
  }
}

}  // namespace xenovent::game
