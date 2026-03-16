#pragma once

#include <array>
#include <cstdint>

#include "input/input_events.h"

namespace xenovent::input {

class ButtonManager {
 public:
  void begin();
  bool pollEvent(InputEvent& outEvent, uint32_t nowMs);

 private:
  struct ButtonState {
    bool stablePressed = false;
    bool lastReadPressed = false;
    bool holdFired = false;
    uint32_t lastChangeMs = 0;
    uint32_t pressedSinceMs = 0;
  };

  bool readRaw(ButtonId button) const;

  std::array<ButtonState, 3> states_{};
  bool comboHoldFired_ = false;
};

}  // namespace xenovent::input
