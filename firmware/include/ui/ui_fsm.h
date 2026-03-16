#pragma once

#include <array>
#include <cstdint>

#include "input/input_events.h"
#include "ui/ui_state.h"

namespace xenovent::ui {

struct UiTransition {
  UiModel model{};
  domain::ActionType triggeredAction = domain::ActionType::None;
  bool illegalTransition = false;
};

class UiFsm {
 public:
  UiFsm();

  UiTransition handleEvent(const input::InputEvent& event, uint32_t nowMs);
  UiTransition tick(uint32_t nowMs);
  void setTransientMessage(const char* msg);

  const UiModel& model() const { return model_; }

 private:
  domain::ActionType cycleAction(domain::ActionType current, int direction) const;

  UiModel model_{};
  std::array<domain::ActionType, 5> actions_ = {
      domain::ActionType::Feed,
      domain::ActionType::Suppress,
      domain::ActionType::Ritual,
      domain::ActionType::Meditate,
      domain::ActionType::Sleep,
  };
};

}  // namespace xenovent::ui
