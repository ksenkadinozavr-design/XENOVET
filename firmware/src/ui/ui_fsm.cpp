#include "ui/ui_fsm.h"

#include <cstddef>

#include "config/ui_config.h"

namespace xenovent::ui {

UiFsm::UiFsm() {
  model_.screen = UiScreen::Main;
  model_.selectedAction = actions_.front();
}

domain::ActionType UiFsm::cycleAction(domain::ActionType current, int direction) const {
  int idx = 0;
  for (std::size_t i = 0; i < actions_.size(); ++i) {
    if (actions_[i] == current) {
      idx = static_cast<int>(i);
      break;
    }
  }
  idx += direction;
  if (idx < 0) idx = static_cast<int>(actions_.size()) - 1;
  if (idx >= static_cast<int>(actions_.size())) idx = 0;
  return actions_[static_cast<std::size_t>(idx)];
}

void UiFsm::setTransientMessage(const char* msg) { model_.transientMessage = msg; }

UiTransition UiFsm::tick(uint32_t nowMs) {
  UiTransition out;
  out.model = model_;
  const bool menuScreen = model_.screen == UiScreen::StatusMenu || model_.screen == UiScreen::ActionMenu ||
                          model_.screen == UiScreen::InfoScreen;
  if (menuScreen && (nowMs - model_.lastInteractionMs) >= config::ui::kMenuAutoReturnMs) {
    model_.screen = UiScreen::Main;
    model_.transientMessage = "auto-return";
    out.model = model_;
  }
  return out;
}

UiTransition UiFsm::handleEvent(const input::InputEvent& event, uint32_t nowMs) {
  UiTransition t;
  model_.lastInteractionMs = nowMs;

  if (event.type == input::InputEventType::ComboUpDownHold) {
    model_.screen = UiScreen::InfoScreen;
    model_.transientMessage = "info";
    t.model = model_;
    return t;
  }

  if (event.type == input::InputEventType::LongPress && event.button == input::ButtonId::Action) {
    model_.screen = UiScreen::Suppress;
    model_.suppressMode = true;
    t.triggeredAction = domain::ActionType::Suppress;
    model_.transientMessage = "suppress";
    t.model = model_;
    return t;
  }

  switch (model_.screen) {
    case UiScreen::Main:
      if (event.type == input::InputEventType::ShortPress && event.button == input::ButtonId::Up) {
        model_.screen = UiScreen::StatusMenu;
      } else if (event.type == input::InputEventType::ShortPress && event.button == input::ButtonId::Down) {
        model_.screen = UiScreen::ActionMenu;
      }
      break;
    case UiScreen::StatusMenu:
    case UiScreen::InfoScreen:
    case UiScreen::Suppress:
      if (event.type == input::InputEventType::ShortPress && event.button == input::ButtonId::Action) {
        model_.screen = UiScreen::Main;
        model_.suppressMode = false;
      } else {
        t.illegalTransition = true;
      }
      break;
    case UiScreen::ActionMenu:
      if (event.type == input::InputEventType::ShortPress && event.button == input::ButtonId::Up) {
        model_.selectedAction = cycleAction(model_.selectedAction, -1);
      } else if (event.type == input::InputEventType::ShortPress &&
                 event.button == input::ButtonId::Down) {
        model_.selectedAction = cycleAction(model_.selectedAction, 1);
      } else if (event.type == input::InputEventType::ShortPress &&
                 event.button == input::ButtonId::Action) {
        t.triggeredAction = model_.selectedAction;
        model_.screen = UiScreen::Main;
      } else {
        t.illegalTransition = true;
      }
      break;
  }

  t.model = model_;
  return t;
}

}  // namespace xenovent::ui
