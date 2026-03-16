#include "input/button_manager.h"

#include <Arduino.h>

#include "config/pins.h"

namespace xenovent::input {

namespace {
int toPin(ButtonId id) {
  switch (id) {
    case ButtonId::Up:
      return config::pins::kButtonUp;
    case ButtonId::Action:
      return config::pins::kButtonAction;
    case ButtonId::Down:
      return config::pins::kButtonDown;
  }
  return -1;
}
}  // namespace

void ButtonManager::begin() {
  pinMode(config::pins::kButtonUp, INPUT_PULLUP);
  pinMode(config::pins::kButtonAction, INPUT_PULLUP);
  pinMode(config::pins::kButtonDown, INPUT_PULLUP);
}

bool ButtonManager::readRaw(ButtonId button) const {
  return digitalRead(toPin(button)) == LOW;  // active low
}

bool ButtonManager::pollEvent(InputEvent& outEvent, uint32_t nowMs) {
  RawButtonState raw;
  raw.upPressed = readRaw(ButtonId::Up);
  raw.actionPressed = readRaw(ButtonId::Action);
  raw.downPressed = readRaw(ButtonId::Down);
  return interpreter_.update(raw, nowMs, outEvent);
}

}  // namespace xenovent::input
