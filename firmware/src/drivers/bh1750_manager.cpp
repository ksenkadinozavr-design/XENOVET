#include "drivers/bh1750_manager.h"

#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>

namespace xenovent::drivers {

namespace {
BH1750 gBh1750;
}

void Bh1750Manager::begin() {
  Wire.begin();
  initialized_ = gBh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  if (!initialized_) {
    Serial.println("[BH1750] init failed (TODO: check wiring/address)");
  }
}

bool Bh1750Manager::read(float& lux) {
  if (!initialized_) {
    lux = 0.0f;
    return false;
  }
  lux = gBh1750.readLightLevel();
  return true;
}

}  // namespace xenovent::drivers
