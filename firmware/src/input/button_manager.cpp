#include "input/button_manager.h"

#include <Arduino.h>

#include "config/constants.h"

namespace xenovent::input {

namespace {
int toPin(ButtonId id) {
  switch (id) {
    case ButtonId::Up:
      return config::kPinButtonUp;
    case ButtonId::Action:
      return config::kPinButtonAction;
    case ButtonId::Down:
      return config::kPinButtonDown;
  }
  return -1;
}

size_t toIndex(ButtonId id) {
  switch (id) {
    case ButtonId::Up:
      return 0;
    case ButtonId::Action:
      return 1;
    case ButtonId::Down:
      return 2;
  }
  return 0;
}
}  // namespace

void ButtonManager::begin() {
  pinMode(config::kPinButtonUp, INPUT_PULLUP);
  pinMode(config::kPinButtonAction, INPUT_PULLUP);
  pinMode(config::kPinButtonDown, INPUT_PULLUP);
}

bool ButtonManager::readRaw(ButtonId button) const {
  const int pin = toPin(button);
  return digitalRead(pin) == LOW;  // active low
}

bool ButtonManager::pollEvent(InputEvent& outEvent, uint32_t nowMs) {
  for (ButtonId id : {ButtonId::Up, ButtonId::Action, ButtonId::Down}) {
    auto& st = states_[toIndex(id)];
    const bool raw = readRaw(id);

    if (raw != st.lastReadPressed) {
      st.lastChangeMs = nowMs;
      st.lastReadPressed = raw;
    }

    if (nowMs - st.lastChangeMs >= config::kDebounceMs && raw != st.stablePressed) {
      st.stablePressed = raw;
      if (st.stablePressed) {
        st.pressedSinceMs = nowMs;
        st.holdFired = false;
      } else {
        if (!st.holdFired) {
          outEvent = InputEvent{InputEventType::ShortPress, id, false};
          return true;
        }
      }
    }

    if (st.stablePressed && !st.holdFired && id == ButtonId::Action &&
        nowMs - st.pressedSinceMs >= config::kHoldSuppressMs) {
      st.holdFired = true;
      outEvent = InputEvent{InputEventType::Hold, id, false};
      return true;
    }
  }

  auto& up = states_[toIndex(ButtonId::Up)];
  auto& down = states_[toIndex(ButtonId::Down)];
  if (up.stablePressed && down.stablePressed) {
    const uint32_t holdMs = nowMs - (up.pressedSinceMs > down.pressedSinceMs ? up.pressedSinceMs : down.pressedSinceMs);
    if (!comboHoldFired_ && holdMs >= config::kHoldInfoComboMs) {
      comboHoldFired_ = true;
      outEvent = InputEvent{InputEventType::ComboHold, ButtonId::Action, true};
      return true;
    }
  } else {
    comboHoldFired_ = false;
  }

  return false;
}

}  // namespace xenovent::input
