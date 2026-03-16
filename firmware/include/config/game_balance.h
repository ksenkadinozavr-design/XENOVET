#pragma once

#include <cstdint>

namespace xenovent::config::balance {

constexpr int kStatMin = 0;
constexpr int kStatMax = 100;

constexpr uint32_t kTickIntervalMs = 5000;
constexpr uint32_t kAutosaveIntervalMs = 30000;

constexpr int kWarningHunger = 75;
constexpr int kWarningInstability = 70;
constexpr int kWarningCorruption = 70;
constexpr int kWarningBonded = 65;

constexpr int kDeathEssence = 0;
constexpr int kDeathInstability = 100;
constexpr int kDeathCorruption = 100;

// Base per-second tick rates.
constexpr int kHungerGainPerSecond = 1;
constexpr int kEssenceDecayPerSecond = 1;
constexpr int kInstabilityGainPerSecond = 1;

// Sensor thresholds.
constexpr float kLuxRecoverThreshold = 450.0f;
constexpr float kShakeInstabilityThreshold = 14.0f;

// Form thresholds.
constexpr uint32_t kAscendMinAgeSeconds = 8 * 60;
constexpr uint32_t kCorruptorMinDeaths = 3;

// Action tuning.
constexpr uint32_t kSleepDurationTicks = 3;
constexpr uint32_t kSuppressDurationTicks = 2;

}  // namespace xenovent::config::balance
