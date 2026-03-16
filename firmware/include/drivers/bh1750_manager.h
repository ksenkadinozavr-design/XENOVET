#pragma once

#include "drivers/adxl345_manager.h"

namespace xenovent::drivers {

class Bh1750Manager {
 public:
  SensorHealth begin();
  bool read(float& lux);
  SensorHealth health() const { return health_; }

 private:
  SensorHealth health_{};
};

}  // namespace xenovent::drivers
