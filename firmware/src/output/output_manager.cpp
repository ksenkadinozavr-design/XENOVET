#include "output/output_manager.h"

#include <Arduino.h>

#include "config/constants.h"

namespace xenovent::output {

void OutputManager::begin() {
  pinMode(config::kPinBuzzer, OUTPUT);
  pinMode(config::kPinVibration, OUTPUT);
  digitalWrite(config::kPinBuzzer, LOW);
  digitalWrite(config::kPinVibration, LOW);
}

void OutputManager::notifyAction(domain::ActionType action) {
  const int pulseMs = (action == domain::ActionType::Suppress) ? 180 : 80;
  digitalWrite(config::kPinVibration, HIGH);
  digitalWrite(config::kPinBuzzer, HIGH);
  delay(pulseMs);
  digitalWrite(config::kPinVibration, LOW);
  digitalWrite(config::kPinBuzzer, LOW);
}

}  // namespace xenovent::output
