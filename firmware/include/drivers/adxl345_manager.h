#pragma once

namespace xenovent::drivers {

struct SensorHealth {
  bool initialized = false;
  bool degraded = false;
};

class Adxl345Manager {
 public:
  SensorHealth begin();
  bool read(float& accelMagnitude);
  SensorHealth health() const { return health_; }

 private:
  SensorHealth health_{};
};

}  // namespace xenovent::drivers
