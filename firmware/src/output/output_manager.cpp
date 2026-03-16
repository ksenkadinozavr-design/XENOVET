#include "output/output_manager.h"

#include <Arduino.h>

#include "config/pins.h"

namespace xenovent::output {

namespace {
void pulse(int pin, int durationMs) {
  digitalWrite(pin, HIGH);
  delay(durationMs);
  digitalWrite(pin, LOW);
}
}  // namespace

void OutputManager::begin() {
  pinMode(config::pins::kBuzzer, OUTPUT);
  pinMode(config::pins::kVibration, OUTPUT);
  digitalWrite(config::pins::kBuzzer, LOW);
  digitalWrite(config::pins::kVibration, LOW);
}

void OutputManager::play(FeedbackPattern pattern) {
  switch (pattern) {
    case FeedbackPattern::Confirm:
      pulse(config::pins::kBuzzer, 50);
      break;
    case FeedbackPattern::Error:
      pulse(config::pins::kVibration, 120);
      pulse(config::pins::kBuzzer, 120);
      break;
    case FeedbackPattern::Warning:
      pulse(config::pins::kVibration, 80);
      break;
    case FeedbackPattern::Mutation:
      pulse(config::pins::kBuzzer, 40);
      delay(20);
      pulse(config::pins::kBuzzer, 60);
      break;
    case FeedbackPattern::Death:
      pulse(config::pins::kVibration, 180);
      delay(30);
      pulse(config::pins::kVibration, 180);
      break;
  }
}

}  // namespace xenovent::output
