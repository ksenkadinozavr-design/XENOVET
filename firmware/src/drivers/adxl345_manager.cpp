#include "drivers/adxl345_manager.h"

#include <Adafruit_ADXL345_U.h>
#include <cmath>
#include <Arduino.h>
#include <Wire.h>

namespace xenovent::drivers {

namespace {
Adafruit_ADXL345_Unified gAdxl(12345);
}

void Adxl345Manager::begin() {
  Wire.begin();
  initialized_ = gAdxl.begin();
  if (!initialized_) {
    Serial.println("[ADXL345] init failed (TODO: check wiring/address)");
    return;
  }
  gAdxl.setRange(ADXL345_RANGE_16_G);
}

bool Adxl345Manager::read(float& accelMagnitude) {
  if (!initialized_) {
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
