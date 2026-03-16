#include <unity.h>

#include "ui/ui_fsm.h"

using namespace xenovent;

void test_main_to_status() {
  ui::UiFsm fsm;
  auto t = fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Up, false}, 100);
  TEST_ASSERT_EQUAL(domain::UiScreen::StatusMenu, t.model.screen);
}

void test_action_menu_confirm() {
  ui::UiFsm fsm;
  fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Down, false}, 100);
  auto t = fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Action, false}, 110);
  TEST_ASSERT_EQUAL(domain::UiScreen::Main, t.model.screen);
  TEST_ASSERT_NOT_EQUAL(domain::ActionType::None, t.triggeredAction);
}

void test_status_ignores_irrelevant_event() {
  ui::UiFsm fsm;
  fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Up, false}, 100);
  auto t = fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Down, false}, 120);
  TEST_ASSERT_FALSE(t.illegalTransition);
  TEST_ASSERT_EQUAL(domain::UiScreen::StatusMenu, t.model.screen);
}

void test_combo_opens_info() {
  ui::UiFsm fsm;
  auto t = fsm.handleEvent({input::InputEventType::ComboUpDownHold, input::ButtonId::Action, false}, 100);
  TEST_ASSERT_EQUAL(domain::UiScreen::InfoScreen, t.model.screen);
}

void test_timeout_auto_return() {
  ui::UiFsm fsm;
  fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Up, false}, 0);
  auto t = fsm.tick(20000);
  TEST_ASSERT_EQUAL(domain::UiScreen::Main, t.model.screen);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_main_to_status);
  RUN_TEST(test_action_menu_confirm);
  RUN_TEST(test_status_ignores_irrelevant_event);
  RUN_TEST(test_combo_opens_info);
  RUN_TEST(test_timeout_auto_return);
  UNITY_END();
}

void loop() {}
