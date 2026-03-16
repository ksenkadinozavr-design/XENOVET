#include "ui/ui_fsm.h"

namespace xenovent::ui {

UiFsm::UiFsm() = default;

domain::ActionType UiFsm::selectedAction() const { return actions_[selectedIndex_]; }

void UiFsm::cycleActionSelection(int direction) {
  const int count = static_cast<int>(actions_.size());
  int idx = static_cast<int>(selectedIndex_) + direction;
  if (idx < 0) idx = count - 1;
  if (idx >= count) idx = 0;
  selectedIndex_ = static_cast<size_t>(idx);
}

UiTransition UiFsm::handleEvent(const input::InputEvent& event) {
  UiTransition t;
  t.nextState = state_;

  if (event.type == input::InputEventType::ComboHold && event.comboUpDown) {
    state_ = UiState::InfoScreen;
    t.nextState = state_;
    return t;
  }

  if (event.type == input::InputEventType::Hold &&
      event.button == input::ButtonId::Action) {
    state_ = UiState::Suppress;
    t.nextState = state_;
    t.forceSuppress = true;
    return t;
  }

  switch (state_) {
    case UiState::Main:
      if (event.type == input::InputEventType::ShortPress && event.button == input::ButtonId::Up) {
        state_ = UiState::StatusMenu;
      } else if (event.type == input::InputEventType::ShortPress &&
                 event.button == input::ButtonId::Down) {
        state_ = UiState::ActionMenu;
      }
      break;
    case UiState::StatusMenu:
    case UiState::InfoScreen:
    case UiState::Suppress:
      if (event.type == input::InputEventType::ShortPress &&
          event.button == input::ButtonId::Action) {
        state_ = UiState::Main;
      }
      break;
    case UiState::ActionMenu:
      if (event.type == input::InputEventType::ShortPress && event.button == input::ButtonId::Up) {
        cycleActionSelection(-1);
      } else if (event.type == input::InputEventType::ShortPress &&
                 event.button == input::ButtonId::Down) {
        cycleActionSelection(1);
      } else if (event.type == input::InputEventType::ShortPress &&
                 event.button == input::ButtonId::Action) {
        t.triggeredAction = selectedAction();
        state_ = UiState::Main;
      }
      break;
  }

  t.nextState = state_;
  return t;
}

}  // namespace xenovent::ui
