#include <unity.h>

#include "input/input_events.h"
#include "ui/ui_fsm.h"

using namespace xenovent;

void test_main_to_status_menu() {
  ui::UiFsm fsm;
  input::InputEvent ev{input::InputEventType::ShortPress, input::ButtonId::Up, false};
  auto tr = fsm.handleEvent(ev);
  TEST_ASSERT_EQUAL(ui::UiState::StatusMenu, tr.nextState);
}

void test_main_to_action_menu() {
  ui::UiFsm fsm;
  input::InputEvent ev{input::InputEventType::ShortPress, input::ButtonId::Down, false};
  auto tr = fsm.handleEvent(ev);
  TEST_ASSERT_EQUAL(ui::UiState::ActionMenu, tr.nextState);
}

void test_action_confirm_returns_main_and_triggers() {
  ui::UiFsm fsm;
  fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Down, false});
  auto tr = fsm.handleEvent({input::InputEventType::ShortPress, input::ButtonId::Action, false});
  TEST_ASSERT_EQUAL(ui::UiState::Main, tr.nextState);
  TEST_ASSERT_NOT_EQUAL(domain::ActionType::None, tr.triggeredAction);
}

void test_combo_opens_info_screen() {
  ui::UiFsm fsm;
  auto tr = fsm.handleEvent({input::InputEventType::ComboHold, input::ButtonId::Action, true});
  TEST_ASSERT_EQUAL(ui::UiState::InfoScreen, tr.nextState);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_main_to_status_menu);
  RUN_TEST(test_main_to_action_menu);
  RUN_TEST(test_action_confirm_returns_main_and_triggers);
  RUN_TEST(test_combo_opens_info_screen);
  UNITY_END();
}

void loop() {}
