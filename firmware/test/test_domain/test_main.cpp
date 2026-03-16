#include <unity.h>

#include "domain/types.h"

using namespace xenovent;

void test_clamp_stat() {
  TEST_ASSERT_EQUAL(0, domain::clampStat(-10));
  TEST_ASSERT_EQUAL(100, domain::clampStat(120));
  TEST_ASSERT_EQUAL(55, domain::clampStat(55));
}

void test_apply_tick_changes_state() {
  domain::CreatureState st;
  st.essence = 50;
  st.hunger = 50;
  st.instability = 20;

  domain::TickContext ctx;
  ctx.dtSeconds = 5;
  ctx.sensors.validLight = true;
  ctx.sensors.lightLux = 600.0f;

  domain::applyTick(st, ctx);

  TEST_ASSERT_TRUE(st.ageSeconds >= 5);
  TEST_ASSERT_TRUE(st.hunger > 50);
  TEST_ASSERT_TRUE(st.essence >= 45);
}

void test_apply_action_feed() {
  domain::CreatureState st;
  st.hunger = 80;
  st.essence = 40;
  domain::applyAction(st, domain::ActionType::Feed);
  TEST_ASSERT_TRUE(st.hunger < 80);
  TEST_ASSERT_TRUE(st.essence > 40);
}

void test_die_and_reset() {
  domain::CreatureState st;
  st.essence = 0;
  TEST_ASSERT_TRUE(domain::shouldDie(st));
  domain::resetAfterDeath(st);
  TEST_ASSERT_EQUAL(domain::CreatureForm::Seed, st.form);
  TEST_ASSERT_EQUAL(1, st.deaths);
  TEST_ASSERT_EQUAL(0, st.ageSeconds);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_clamp_stat);
  RUN_TEST(test_apply_tick_changes_state);
  RUN_TEST(test_apply_action_feed);
  RUN_TEST(test_die_and_reset);
  UNITY_END();
}

void loop() {}
