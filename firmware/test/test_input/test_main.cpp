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

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_action_long_press);
  RUN_TEST(test_combo_hold);
  UNITY_END();
}

void loop() {}
