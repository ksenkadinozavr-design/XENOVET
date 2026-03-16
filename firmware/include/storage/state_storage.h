#pragma once

#include "domain/types.h"

namespace xenovent::storage {

class StateStorage {
 public:
  void begin();
  void saveState(const domain::CreatureState& state);
  domain::CreatureState loadState();
  domain::CreatureState defaultState() const;
  void factoryReset();

 private:
  bool initialized_ = false;
};

}  // namespace xenovent::storage
