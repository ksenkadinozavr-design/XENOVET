#pragma once

#include "domain/types.h"

namespace xenovent::storage {

struct StorageLoadResult {
  domain::CreatureState state{};
  bool loadedFromNvs = false;
  bool versionMismatch = false;
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
