#include "ui/renderer.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "config/constants.h"

namespace xenovent::ui {

namespace {
Adafruit_ST7789 tft(config::kTftCs, config::kTftDc, config::kTftRst);

const char* toScreenName(UiState state) {
  switch (state) {
    case UiState::Main:
      return "Main";
    case UiState::StatusMenu:
      return "Status";
    case UiState::ActionMenu:
      return "Actions";
    case UiState::InfoScreen:
      return "Info";
    case UiState::Suppress:
      return "Suppress";
  }
  return "Unknown";
}
}  // namespace

void Renderer::begin() {
  tft.init(config::kTftWidth, config::kTftHeight);
  initialized_ = true;
  if (!initialized_) {
    Serial.println("[Renderer] ST7789 init failed, fallback to serial only");
    return;
  }
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);
}

void Renderer::drawHeader(const char* title) {
  tft.fillRect(0, 0, config::kTftWidth, 24, ST77XX_BLUE);
  tft.setCursor(6, 6);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLUE);
  tft.setTextSize(2);
  tft.print(title);
}

void Renderer::drawCreature(const domain::CreatureState& creature) {
  tft.setCursor(6, 34);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.printf("Form: %s", domain::toString(creature.form));
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

void Renderer::drawAction(domain::ActionType action) {
  tft.setCursor(6, 220);
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.setTextSize(2);
  tft.printf("Action: %s", domain::toString(action));
}

void Renderer::render(UiState state, const domain::CreatureState& creature,
                      domain::ActionType selectedAction) {
  if (!initialized_) {
    Serial.printf("[UI] %s | E:%d H:%d I:%d B:%d C:%d A:%s\n", toScreenName(state), creature.essence,
                  creature.hunger, creature.instability, creature.bond, creature.corruption,
                  domain::toString(selectedAction));
    return;
  }

  tft.fillScreen(ST77XX_BLACK);
  drawHeader(toScreenName(state));
  drawCreature(creature);
  drawStatLine("ESSENCE", creature.essence, 70);
  drawStatLine("HUNGER", creature.hunger, 90);
  drawStatLine("INSTAB", creature.instability, 110);
  drawStatLine("BOND", creature.bond, 130);
  drawStatLine("CORRUPT", creature.corruption, 150);

  if (state == UiState::ActionMenu) {
    drawAction(selectedAction);
  }
}

}  // namespace xenovent::ui
