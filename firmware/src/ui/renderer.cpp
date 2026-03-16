#include "ui/renderer.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Arduino.h>

#include "config/constants.h"
#include "domain/types.h"
#include "ui/animation_state.h"
#include "ui/icons.h"
#include "ui/layout.h"
#include "ui/theme.h"
#include "utils/logger.h"

namespace xenovent::ui {

namespace {
Adafruit_ST7789 tft(config::kTftCs, config::kTftDc, config::kTftRst);
const char* screenName(UiScreen screen) { return domain::toString(screen); }
}

void Renderer::begin() {
  tft.init(config::kTftWidth, config::kTftHeight);
  initialized_ = true;
  tft.setRotation(0);
  tft.fillScreen(theme::kBackground);
  tft.setTextWrap(false);
  utils::info("Renderer", "ST7789 initialized");
}

void Renderer::drawHeader(const char* title) {
  tft.fillRect(0, 0, config::kTftWidth, layout::kHeaderH, theme::kHeaderBg);
  tft.setCursor(6, 6);
  tft.setTextColor(theme::kHeaderText, theme::kHeaderBg);
  tft.setTextSize(2);
  tft.print(title);
}

void Renderer::drawFormIcon(domain::CreatureForm form, int pulse) {
  const auto icon = iconForForm(form);
  const int x = layout::kFormIconX;
  const int y = layout::kFormIconY;
  const int size = icon.size + pulse;
  const uint16_t color = theme::kAccentText;

  switch (icon.type) {
    case IconPrimitive::Type::Circle:
      tft.drawCircle(x, y, size, color);
      break;
    case IconPrimitive::Type::Rect:
      tft.drawRect(x - size, y - size, size * 2, size * 2, color);
      break;
    case IconPrimitive::Type::Triangle:
      tft.drawTriangle(x, y - size, x - size, y + size, x + size, y + size, color);
      break;
  }
}

void Renderer::drawStatLine(const char* name, int value, int y) {
  tft.setCursor(6, y);
  tft.setTextColor(theme::kPrimaryText, theme::kBackground);
  tft.setTextSize(1);
  tft.printf("%s: %3d", name, value);

  const int barX = 90;
  const int barW = 120;
  const int fillW = (barW * value) / 100;
  tft.drawRect(barX, y, barW, 8, theme::kPrimaryText);
  tft.fillRect(barX + 1, y + 1, fillW - 2 > 0 ? fillW - 2 : 0, 6, theme::kAccentText);
}

void Renderer::drawWarnings(const domain::UiFlags& flags, bool blinkOn) {
  tft.setCursor(6, 200);
  tft.setTextColor(blinkOn ? theme::kWarningText : theme::kPrimaryText, theme::kBackground);
  tft.setTextSize(1);
  tft.print("WARN:");
  if (flags.starving) tft.print(" STARVE");
  if (flags.unstable) tft.print(" UNSTABLE");
  if (flags.highCorruption) tft.print(" CORRUPT");
  if (flags.nearDeath) tft.print(" NEAR_DEATH");
}

void Renderer::renderMainScreen(const RenderModel& model, uint32_t nowMs) {
  const auto anim = buildAnimationState(nowMs);
  drawHeader("Main");
  tft.setCursor(6, 32);
  tft.setTextColor(theme::kSuccessText, theme::kBackground);
  tft.setTextSize(1);
  tft.printf("Form:%s Age:%lu Deaths:%lu", domain::toString(model.creature.form),
             static_cast<unsigned long>(model.creature.ageSeconds),
             static_cast<unsigned long>(model.creature.deaths));

  drawFormIcon(model.creature.form, anim.pulse);
  drawStatLine("ESSENCE", model.creature.essence, layout::kStatsStartY);
  drawStatLine("HUNGER", model.creature.hunger, layout::kStatsStartY + layout::kStatsStepY);
  drawStatLine("INSTAB", model.creature.instability,
               layout::kStatsStartY + layout::kStatsStepY * 2);
  drawStatLine("BOND", model.creature.bond, layout::kStatsStartY + layout::kStatsStepY * 3);
  drawStatLine("CORRUPT", model.creature.corruption,
               layout::kStatsStartY + layout::kStatsStepY * 4);
  drawWarnings(model.flags, anim.blinkOn);
}

void Renderer::renderStatusMenu(const RenderModel& model) {
  drawHeader("Status");
  tft.setCursor(6, 40);
  tft.setTextColor(theme::kPrimaryText, theme::kBackground);
  tft.setTextSize(1);
  tft.printf("Sleep ticks: %lu", static_cast<unsigned long>(model.creature.sleepTicksRemaining));
  tft.setCursor(6, 55);
  tft.printf("Suppress ticks: %lu", static_cast<unsigned long>(model.creature.suppressTicksRemaining));
}

void Renderer::renderActionMenu(const RenderModel& model) {
  drawHeader("Actions");
  tft.setCursor(6, 40);
  tft.setTextColor(theme::kWarningText, theme::kBackground);
  tft.setTextSize(2);
  const auto action = model.ui.selectedAction;
  tft.printf("%s", domain::toString(action));
  const auto cooldown = domain::getRemainingCooldown(model.creature, action);
  tft.setCursor(6, 70);
  tft.setTextSize(1);
  if (cooldown > 0) {
    tft.setTextColor(theme::kDangerText, theme::kBackground);
    tft.printf("Cooldown: %lu", static_cast<unsigned long>(cooldown));
  } else {
    tft.setTextColor(theme::kSuccessText, theme::kBackground);
    tft.print("Ready");
  }
}

void Renderer::renderInfoScreen(const RenderModel& model) {
  drawHeader("Info");
  tft.setCursor(6, 40);
  tft.setTextColor(theme::kPrimaryText, theme::kBackground);
  tft.setTextSize(1);
  tft.printf("Screen: %s", screenName(model.ui.screen));
  tft.setCursor(6, 54);
  tft.printf("Form: %s", domain::toString(model.creature.form));
}

void Renderer::renderSuppressScreen(const RenderModel& model) {
  drawHeader("Suppress");
  tft.setCursor(6, 44);
  tft.setTextColor(theme::kDangerText, theme::kBackground);
  tft.setTextSize(2);
  tft.print("SUPPRESSION");
  tft.setCursor(6, 70);
  tft.setTextSize(1);
  tft.printf("ticks left: %lu", static_cast<unsigned long>(model.creature.suppressTicksRemaining));
}

void Renderer::renderTransientOverlay(const RenderModel& model, bool blinkOn) {
  if (model.ui.transientMessage == nullptr || model.ui.transientMessage[0] == '\0') {
    return;
  }
  const uint16_t bg = blinkOn ? theme::kHeaderBg : theme::kBackground;
  tft.fillRect(0, config::kTftHeight - 20, config::kTftWidth, 20, bg);
  tft.setCursor(4, config::kTftHeight - 14);
  tft.setTextColor(theme::kHeaderText, bg);
  tft.setTextSize(1);
  tft.print(model.ui.transientMessage);
}

void Renderer::render(const RenderModel& model) {
  if (!initialized_) {
    utils::warn("Renderer", "serial fallback screen=%s action=%s", screenName(model.ui.screen),
                domain::toString(model.ui.selectedAction));
    return;
  }

  const uint32_t nowMs = millis();
  const auto anim = buildAnimationState(nowMs);

  tft.fillScreen(theme::kBackground);
  switch (model.ui.screen) {
    case UiScreen::Main:
      renderMainScreen(model, nowMs);
      break;
    case UiScreen::StatusMenu:
      renderStatusMenu(model);
      break;
    case UiScreen::ActionMenu:
      renderActionMenu(model);
      break;
    case UiScreen::InfoScreen:
      renderInfoScreen(model);
      break;
    case UiScreen::Suppress:
      renderSuppressScreen(model);
      break;
  }
  renderTransientOverlay(model, anim.blinkOn);
}

}  // namespace xenovent::ui
