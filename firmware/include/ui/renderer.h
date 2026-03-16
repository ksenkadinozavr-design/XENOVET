#pragma once

#include "domain/types.h"
#include "ui/ui_state.h"

namespace xenovent::ui {

struct RenderModel {
  UiModel ui{};
  domain::CreatureState creature{};
  domain::UiFlags flags{};
};

class Renderer {
 public:
  void begin();
  void render(const RenderModel& model);

 private:
  void drawHeader(const char* title);
  void drawStatLine(const char* name, int value, int y);
  void drawWarnings(const domain::UiFlags& flags, bool blinkOn);
  void drawFormIcon(domain::CreatureForm form, int pulse);

  void renderMainScreen(const RenderModel& model, uint32_t nowMs);
  void renderStatusMenu(const RenderModel& model);
  void renderActionMenu(const RenderModel& model);
  void renderInfoScreen(const RenderModel& model);
  void renderSuppressScreen(const RenderModel& model);
  void renderTransientOverlay(const RenderModel& model, bool blinkOn);

  bool initialized_ = false;
};

}  // namespace xenovent::ui
