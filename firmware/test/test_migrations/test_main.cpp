#include <unity.h>

#include "config/app_config.h"
#include "storage/migrations.h"

using namespace xenovent;

void test_migration_v1_to_v3_success() {
  storage::PersistedStateEnvelope env;
  env.schemaVersion = 1;
  env.state.essence = 10;

  auto result = storage::migrateToLatest(env, config::app::kStateVersion);
  TEST_ASSERT_TRUE(result.success);
  TEST_ASSERT_EQUAL(storage::RecoveryStatus::MigrationApplied, result.status);
  TEST_ASSERT_EQUAL(config::app::kStateVersion, env.schemaVersion);
}

void test_migration_unsupported_version() {
  storage::PersistedStateEnvelope env;
  env.schemaVersion = 99;
  auto result = storage::migrateToLatest(env, config::app::kStateVersion);
  TEST_ASSERT_FALSE(result.success);
  TEST_ASSERT_EQUAL(storage::RecoveryStatus::UnsupportedVersion, result.status);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_migration_v1_to_v3_success);
  RUN_TEST(test_migration_unsupported_version);
  UNITY_END();
}

void loop() {}
