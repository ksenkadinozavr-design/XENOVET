#pragma once

#include <cstdint>

#include "domain/actions.h"
#include "domain/types.h"

namespace xenovent::ui {

using UiScreen = xenovent::domain::UiScreen;

struct UiModel {
  UiScreen screen = UiScreen::Main;
  domain::ActionType selectedAction = domain::ActionType::Feed;
  const char* transientMessage = "";
  bool suppressMode = false;
  uint32_t lastInteractionMs = 0;
};

}  // namespace xenovent::ui
