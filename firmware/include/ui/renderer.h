#pragma once

#include "domain/types.h"
#include "ui/ui_state.h"

namespace xenovent::ui {

class Renderer {
 public:
  void begin();
  void render(UiState state, const domain::CreatureState& creature,
              domain::ActionType selectedAction);

 private:
  void drawHeader(const char* title);
  void drawCreature(const domain::CreatureState& creature);
  void drawStatLine(const char* name, int value, int y);
  void drawAction(domain::ActionType action);

  bool initialized_ = false;
};

}  // namespace xenovent::ui
