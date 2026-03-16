#include "drivers/adxl345_manager.h"

#include <Adafruit_ADXL345_U.h>
#include <Arduino.h>
#include <Wire.h>
#include <cmath>

#include "utils/logger.h"

namespace xenovent::drivers {

namespace {
Adafruit_ADXL345_Unified gAdxl(12345);
}

SensorHealth Adxl345Manager::begin() {
  Wire.begin();
  health_.initialized = gAdxl.begin();
  health_.degraded = !health_.initialized;
  if (!health_.initialized) {
    utils::warn("ADXL345", "init failed, using safe defaults");
    return health_;
  }
  gAdxl.setRange(ADXL345_RANGE_16_G);
  utils::info("ADXL345", "ready");
  return health_;
}

bool Adxl345Manager::read(float& accelMagnitude) {
  if (!health_.initialized) {
    accelMagnitude = 0.0f;
    return false;
  }

  sensors_event_t event;
  gAdxl.getEvent(&event);
  const float x = event.acceleration.x;
  const float y = event.acceleration.y;
  const float z = event.acceleration.z;
  accelMagnitude = sqrtf((x * x) + (y * y) + (z * z));
  return true;
}

}  // namespace xenovent::drivers
