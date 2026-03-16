#pragma once

#include <array>
#include <cstdint>

#include "input/input_events.h"
#include "input/input_interpreter.h"

namespace xenovent::input {

class ButtonManager {
 public:
  void begin();
  bool pollEvent(InputEvent& outEvent, uint32_t nowMs);

 private:
  bool readRaw(ButtonId button) const;

  InputInterpreter interpreter_{};
};

}  // namespace xenovent::input
