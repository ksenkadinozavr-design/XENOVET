#include <unity.h>

#include "domain/types.h"

using namespace xenovent;

void test_clamp_and_normalize() {
  domain::CreatureState st;
  st.essence = -10;
  st.hunger = 140;
  st.instability = 20;
  st.bond = 999;
  st.corruption = -5;

  auto norm = domain::normalizeState(st);
  TEST_ASSERT_EQUAL(0, norm.essence);
  TEST_ASSERT_EQUAL(100, norm.hunger);
  TEST_ASSERT_EQUAL(100, norm.bond);
  TEST_ASSERT_EQUAL(0, norm.corruption);
}

void test_apply_tick_sensor_recovery() {
  domain::CreatureState st = domain::buildDefaultState();
  st.essence = 40;

  domain::TickContext ctx;
  ctx.dtSeconds = 5;
  ctx.sensors.validLight = true;
  ctx.sensors.lightLux = 700.0f;

  auto after = domain::applyTick(st, ctx);
  TEST_ASSERT_TRUE(after.ageSeconds >= 5);
  TEST_ASSERT_TRUE(after.essence >= 40);
}

void test_actions_all() {
  domain::CreatureState st = domain::buildDefaultState();

  auto feed = domain::applyAction(st, domain::ActionType::Feed);
  TEST_ASSERT_TRUE(feed.accepted);
  TEST_ASSERT_TRUE(feed.after.hunger < st.hunger);

  auto suppress = domain::applyAction(st, domain::ActionType::Suppress);
  TEST_ASSERT_TRUE(suppress.accepted);
  TEST_ASSERT_TRUE(suppress.after.suppressTicksRemaining > 0);

  auto ritual = domain::applyAction(st, domain::ActionType::Ritual);
  TEST_ASSERT_TRUE(ritual.accepted);
  TEST_ASSERT_TRUE(ritual.after.corruption > st.corruption);

  auto meditate = domain::applyAction(st, domain::ActionType::Meditate);
  TEST_ASSERT_TRUE(meditate.accepted);
  TEST_ASSERT_TRUE(meditate.after.instability < st.instability);

  auto sleep = domain::applyAction(st, domain::ActionType::Sleep);
  TEST_ASSERT_TRUE(sleep.accepted);
  TEST_ASSERT_TRUE(sleep.after.sleepTicksRemaining > 0);
}

void test_should_die_and_reset() {
  domain::CreatureState st = domain::buildDefaultState();
  st.essence = 0;
  auto death = domain::shouldDie(st);
  TEST_ASSERT_TRUE(death.died);
  auto reset = domain::resetAfterDeath(st);
  TEST_ASSERT_EQUAL(1, reset.deaths);
  TEST_ASSERT_EQUAL(domain::CreatureForm::Seed, reset.form);
}

void test_mutation_transition() {
  domain::CreatureState st = domain::buildDefaultState();
  st.corruption = 80;
  st.instability = 85;

  auto m = domain::checkMutation(st);
  TEST_ASSERT_TRUE(m.mutated);
  TEST_ASSERT_EQUAL(domain::CreatureForm::Shade, m.to);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_clamp_and_normalize);
  RUN_TEST(test_apply_tick_sensor_recovery);
  RUN_TEST(test_actions_all);
  RUN_TEST(test_should_die_and_reset);
  RUN_TEST(test_mutation_transition);
  UNITY_END();
}

void loop() {}
