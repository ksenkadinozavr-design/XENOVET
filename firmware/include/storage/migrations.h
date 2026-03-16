#pragma once

#include <cstdint>

#include "domain/types.h"

namespace xenovent::storage {

enum class RecoveryStatus : uint8_t {
  None = 0,
  MigrationApplied,
  MigrationFailed,
  FallbackToDefaults,
  CorruptedRecovered,
  UnsupportedVersion,
};

struct PersistedStateEnvelope {
  uint32_t schemaVersion = 0;
  domain::CreatureState state{};
};

struct MigrationResult {
  bool success = true;
  uint32_t fromVersion = 0;
  uint32_t toVersion = 0;
  RecoveryStatus status = RecoveryStatus::None;
  const char* message = "ok";
};

MigrationResult migrateToLatest(PersistedStateEnvelope& envelope, uint32_t latestVersion);

}  // namespace xenovent::storage
