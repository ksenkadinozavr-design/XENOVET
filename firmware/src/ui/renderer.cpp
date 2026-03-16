#include "ui/renderer.h"

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Arduino.h>

#include "config/constants.h"
#include "utils/logger.h"

namespace xenovent::ui {

namespace {
Adafruit_ST7789 tft(config::kTftCs, config::kTftDc, config::kTftRst);

const char* screenName(UiScreen screen) { return domain::toString(screen); }
}  // namespace

void Renderer::begin() {
  tft.init(config::kTftWidth, config::kTftHeight);
  initialized_ = true;
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);
  utils::info("Renderer", "ST7789 initialized");
}

void Renderer::drawHeader(const char* title) {
  tft.fillRect(0, 0, config::kTftWidth, 24, ST77XX_BLUE);
  tft.setCursor(6, 6);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLUE);
  tft.setTextSize(2);
  tft.print(title);
}

void Renderer::drawStatLine(const char* name, int value, int y) {
  tft.setCursor(6, y);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.printf("%s: %3d", name, value);

  const int barX = 90;
  const int barW = 120;
  const int fillW = (barW * value) / 100;
  tft.drawRect(barX, y, barW, 8, ST77XX_WHITE);
  tft.fillRect(barX + 1, y + 1, fillW - 2 > 0 ? fillW - 2 : 0, 6, ST77XX_CYAN);
}

void Renderer::drawWarnings(const domain::UiFlags& flags) {
  tft.setCursor(6, 200);
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.print("WARN:");
  if (flags.starving) tft.print(" STARVE");
  if (flags.unstable) tft.print(" UNSTABLE");
  if (flags.highCorruption) tft.print(" CORRUPT");
  if (flags.nearDeath) tft.print(" NEAR_DEATH");
}

void Renderer::renderMainScreen(const RenderModel& model) {
  drawHeader("Main");
  tft.setCursor(6, 32);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.printf("Form:%s Age:%lu Deaths:%lu", domain::toString(model.creature.form),
             static_cast<unsigned long>(model.creature.ageSeconds),
             static_cast<unsigned long>(model.creature.deaths));

  drawStatLine("ESSENCE", model.creature.essence, 60);
  drawStatLine("HUNGER", model.creature.hunger, 80);
  drawStatLine("INSTAB", model.creature.instability, 100);
  drawStatLine("BOND", model.creature.bond, 120);
  drawStatLine("CORRUPT", model.creature.corruption, 140);
  drawWarnings(model.flags);
}

void Renderer::renderStatusMenu(const RenderModel& model) {
  drawHeader("Status");
  tft.setCursor(6, 40);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.printf("Sleep ticks: %lu", static_cast<unsigned long>(model.creature.sleepTicksRemaining));
  tft.setCursor(6, 55);
  tft.printf("Suppress ticks: %lu", static_cast<unsigned long>(model.creature.suppressTicksRemaining));
}

void Renderer::renderActionMenu(const RenderModel& model) {
  drawHeader("Actions");
  tft.setCursor(6, 40);
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.printf("%s", domain::toString(model.ui.selectedAction));
}

void Renderer::renderInfoScreen(const RenderModel& model) {
  drawHeader("Info");
  tft.setCursor(6, 40);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setTextSize(1);
  tft.printf("Screen: %s", screenName(model.ui.screen));
  tft.setCursor(6, 54);
  tft.printf("Form: %s", domain::toString(model.creature.form));
}

void Renderer::renderSuppressScreen(const RenderModel& model) {
  drawHeader("Suppress");
  tft.setCursor(6, 44);
  tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.print("SUPPRESSION");
  tft.setCursor(6, 70);
  tft.setTextSize(1);
  tft.printf("ticks left: %lu", static_cast<unsigned long>(model.creature.suppressTicksRemaining));
}

void Renderer::renderTransientOverlay(const RenderModel& model) {
  if (model.ui.transientMessage == nullptr || model.ui.transientMessage[0] == '\0') {
    return;
  }
  tft.fillRect(0, config::kTftHeight - 20, config::kTftWidth, 20, ST77XX_BLUE);
  tft.setCursor(4, config::kTftHeight - 14);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLUE);
  tft.setTextSize(1);
  tft.print(model.ui.transientMessage);
}

void Renderer::render(const RenderModel& model) {
  if (!initialized_) {
    utils::warn("Renderer", "serial fallback screen=%s action=%s", screenName(model.ui.screen),
                domain::toString(model.ui.selectedAction));
    return;
  }

  tft.fillScreen(ST77XX_BLACK);
  switch (model.ui.screen) {
    case UiScreen::Main:
      renderMainScreen(model);
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
  renderTransientOverlay(model);
}

}  // namespace xenovent::ui
