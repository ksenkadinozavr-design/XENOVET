#pragma once

namespace xenovent::input {

enum class ButtonId {
  Up,
  Action,
  Down,
};

enum class InputEventType {
  ShortPress,
  Hold,
  ComboHold,
};

struct InputEvent {
  InputEventType type;
  ButtonId button = ButtonId::Action;
  bool comboUpDown = false;
};

}  // namespace xenovent::input
