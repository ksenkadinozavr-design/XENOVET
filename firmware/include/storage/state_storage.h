#pragma once

#include "domain/types.h"
#include "storage/migrations.h"

namespace xenovent::storage {

struct StorageLoadResult {
  domain::CreatureState state{};
  bool loadedFromNvs = false;
  bool versionMismatch = false;
  MigrationResult migration{};
  RecoveryStatus recoveryStatus = RecoveryStatus::None;
};

class StateStorage {
 public:
  void begin();
  bool hasSavedState();
  bool saveState(const domain::CreatureState& state);
  StorageLoadResult loadState();
  domain::CreatureState defaultState() const;
  bool resetState();

 private:
  bool initialized_ = false;
};

}  // namespace xenovent::storage
