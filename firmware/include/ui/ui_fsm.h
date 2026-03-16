#pragma once

#include <array>

#include "domain/actions.h"
#include "input/input_events.h"
#include "ui/ui_state.h"

namespace xenovent::ui {

struct UiTransition {
  UiState nextState = UiState::Main;
  domain::ActionType triggeredAction = domain::ActionType::None;
  bool forceSuppress = false;
};

class UiFsm {
 public:
  UiFsm();

  UiTransition handleEvent(const input::InputEvent& event);

  UiState currentState() const { return state_; }
  domain::ActionType selectedAction() const;

 private:
  void cycleActionSelection(int direction);

  UiState state_ = UiState::Main;
  std::array<domain::ActionType, 5> actions_ = {
      domain::ActionType::Feed,
      domain::ActionType::Suppress,
      domain::ActionType::Ritual,
      domain::ActionType::Meditate,
      domain::ActionType::Sleep,
  };
  size_t selectedIndex_ = 0;
};

}  // namespace xenovent::ui
