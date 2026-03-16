#pragma once

#include <cstdint>

namespace xenovent::input {

enum class ButtonId : uint8_t {
  Up = 0,
  Action,
  Down,
};

enum class InputEventType : uint8_t {
  None = 0,
  ShortPress,
  LongPress,
  ComboUpDownHold,
};

struct InputEvent {
  InputEventType type = InputEventType::None;
  ButtonId button = ButtonId::Action;
  bool consumed = false;
};

}  // namespace xenovent::input
