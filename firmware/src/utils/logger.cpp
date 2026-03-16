#include "utils/logger.h"

#include <Arduino.h>

namespace xenovent::utils {

void initLogger() { Serial.begin(115200); }

}  // namespace xenovent::utils
