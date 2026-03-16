#pragma once

#include "domain/actions.h"

namespace xenovent::output {

class OutputManager {
 public:
  void begin();
  void notifyAction(domain::ActionType action);
};

}  // namespace xenovent::output
