#pragma once

#include <array>
#include <cstdint>

#include "input/input_events.h"

namespace xenovent::input {

struct RawButtonState {
  bool upPressed = false;
  bool actionPressed = false;
  bool downPressed = false;
};

class InputInterpreter {
 public:
  bool update(const RawButtonState& raw, uint32_t nowMs, InputEvent& outEvent);

 private:
  struct ButtonTrack {
    bool stablePressed = false;
    bool rawPressed = false;
    bool holdFired = false;
    uint32_t lastChangeMs = 0;
    uint32_t pressedSinceMs = 0;
  };

  bool emitButtonEvent(ButtonId id, bool rawPressed, uint32_t nowMs, InputEvent& outEvent);

  std::array<ButtonTrack, 3> tracks_{};
  bool comboFired_ = false;
};

}  // namespace xenovent::input
