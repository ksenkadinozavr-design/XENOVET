#include "storage/migrations.h"

namespace xenovent::storage {

namespace {

bool migrateV1ToV2(PersistedStateEnvelope& env) {
  env.state.sleepTicksRemaining = 0;
  env.state.suppressTicksRemaining = 0;
  env.schemaVersion = 2;
  return true;
}

bool migrateV2ToV3(PersistedStateEnvelope& env) {
  env.state.cooldowns = {};
  env.schemaVersion = 3;
  return true;
}

}  // namespace

MigrationResult migrateToLatest(PersistedStateEnvelope& envelope, uint32_t latestVersion) {
  MigrationResult result;
  result.fromVersion = envelope.schemaVersion;
  result.toVersion = envelope.schemaVersion;

  if (envelope.schemaVersion == 0 || envelope.schemaVersion > latestVersion) {
    result.success = false;
    result.status = RecoveryStatus::UnsupportedVersion;
    result.message = "unsupported_version";
    return result;
  }

  if (envelope.schemaVersion == latestVersion) {
    result.success = true;
    result.status = RecoveryStatus::None;
    result.message = "up_to_date";
    return result;
  }

  while (envelope.schemaVersion < latestVersion) {
    bool ok = false;
    if (envelope.schemaVersion == 1) {
      ok = migrateV1ToV2(envelope);
    } else if (envelope.schemaVersion == 2) {
      ok = migrateV2ToV3(envelope);
    }

    if (!ok) {
      result.success = false;
      result.status = RecoveryStatus::MigrationFailed;
      result.message = "migration_failed";
      result.toVersion = envelope.schemaVersion;
      return result;
    }
  }

  result.success = true;
  result.toVersion = envelope.schemaVersion;
  result.status = RecoveryStatus::MigrationApplied;
  result.message = "migration_applied";
  return result;
}

}  // namespace xenovent::storage
