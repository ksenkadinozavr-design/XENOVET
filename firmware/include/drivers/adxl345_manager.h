#pragma once

namespace xenovent::drivers {

class Adxl345Manager {
 public:
  void begin();
  bool read(float& accelMagnitude);

 private:
  bool initialized_ = false;
};

}  // namespace xenovent::drivers
