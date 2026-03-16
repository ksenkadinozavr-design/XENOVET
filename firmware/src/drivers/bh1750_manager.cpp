#include "drivers/bh1750_manager.h"

#include <Arduino.h>
#include <BH1750.h>
#include <Wire.h>

#include "utils/logger.h"

namespace xenovent::drivers {

namespace {
BH1750 gBh1750;
}

SensorHealth Bh1750Manager::begin() {
  Wire.begin();
  health_.initialized = gBh1750.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  health_.degraded = !health_.initialized;
  if (!health_.initialized) {
    utils::warn("BH1750", "init failed, using safe defaults");
  } else {
    utils::info("BH1750", "ready");
  }
  return health_;
}

bool Bh1750Manager::read(float& lux) {
  if (!health_.initialized) {
    lux = 0.0f;
    return false;
  }
  lux = gBh1750.readLightLevel();
  return true;
}

}  // namespace xenovent::drivers
