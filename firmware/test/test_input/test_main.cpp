#include <unity.h>

#include "input/input_interpreter.h"

using namespace xenovent;

void test_action_long_press() {
  input::InputInterpreter it;
  input::InputEvent ev;
  input::RawButtonState raw;

  raw.actionPressed = true;
  TEST_ASSERT_FALSE(it.update(raw, 0, ev));
  TEST_ASSERT_FALSE(it.update(raw, 1000, ev));
  TEST_ASSERT_TRUE(it.update(raw, 3100, ev));
  TEST_ASSERT_EQUAL(input::InputEventType::LongPress, ev.type);
}

void test_combo_hold() {
  input::InputInterpreter it;
  input::InputEvent ev;
  input::RawButtonState raw;
  raw.upPressed = true;
  raw.downPressed = true;

  TEST_ASSERT_FALSE(it.update(raw, 0, ev));
  TEST_ASSERT_FALSE(it.update(raw, 1000, ev));
  TEST_ASSERT_TRUE(it.update(raw, 2200, ev));
  TEST_ASSERT_EQUAL(input::InputEventType::ComboUpDownHold, ev.type);
}

void test_debounce_ignores_bounce() {
  input::InputInterpreter it;
  input::InputEvent ev;
  input::RawButtonState raw;

  raw.upPressed = true;
  TEST_ASSERT_FALSE(it.update(raw, 0, ev));
  raw.upPressed = false;
  TEST_ASSERT_FALSE(it.update(raw, 10, ev));
  raw.upPressed = true;
  TEST_ASSERT_FALSE(it.update(raw, 20, ev));
  raw.upPressed = false;
  TEST_ASSERT_FALSE(it.update(raw, 40, ev));
}

void test_combo_release_no_short_events() {
  input::InputInterpreter it;
  input::InputEvent ev;
  input::RawButtonState raw;
  raw.upPressed = true;
  raw.downPressed = true;

  TEST_ASSERT_FALSE(it.update(raw, 0, ev));
  TEST_ASSERT_TRUE(it.update(raw, 2200, ev));
  TEST_ASSERT_EQUAL(input::InputEventType::ComboUpDownHold, ev.type);

  raw.upPressed = false;
  raw.downPressed = false;
  TEST_ASSERT_FALSE(it.update(raw, 2300, ev));
  TEST_ASSERT_FALSE(it.update(raw, 2400, ev));
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_action_long_press);
  RUN_TEST(test_combo_hold);
  RUN_TEST(test_debounce_ignores_bounce);
  RUN_TEST(test_combo_release_no_short_events);
  UNITY_END();
}

void loop() {}
