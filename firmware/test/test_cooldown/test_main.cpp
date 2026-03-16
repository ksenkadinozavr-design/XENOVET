#include <unity.h>

#include "domain/types.h"

using namespace xenovent;

void test_action_cooldown_start_and_decay() {
  domain::CreatureState st = domain::buildDefaultState();

  auto res = domain::applyAction(st, domain::ActionType::Ritual);
  TEST_ASSERT_TRUE(res.accepted);
  TEST_ASSERT_TRUE(domain::getRemainingCooldown(res.after, domain::ActionType::Ritual) > 0);

  domain::TickContext ctx;
  ctx.dtSeconds = 1;
  auto next = domain::applyTick(res.after, ctx);
  TEST_ASSERT_TRUE(domain::getRemainingCooldown(next, domain::ActionType::Ritual) <
                   domain::getRemainingCooldown(res.after, domain::ActionType::Ritual));
}

void test_action_unavailable_during_cooldown() {
  domain::CreatureState st = domain::buildDefaultState();
  auto first = domain::applyAction(st, domain::ActionType::Sleep);
  TEST_ASSERT_TRUE(first.accepted);

  auto second = domain::applyAction(first.after, domain::ActionType::Sleep);
  TEST_ASSERT_FALSE(second.accepted);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_action_cooldown_start_and_decay);
  RUN_TEST(test_action_unavailable_during_cooldown);
  UNITY_END();
}

void loop() {}
