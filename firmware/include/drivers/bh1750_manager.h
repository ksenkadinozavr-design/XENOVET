#pragma once

namespace xenovent::drivers {

class Bh1750Manager {
 public:
  void begin();
  bool read(float& lux);

 private:
  bool initialized_ = false;
};

}  // namespace xenovent::drivers
