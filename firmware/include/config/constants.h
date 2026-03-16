#pragma once

#include <cstdint>

namespace xenovent::config {

constexpr int kStatMin = 0;
constexpr int kStatMax = 100;

constexpr uint32_t kFastLoopMs = 33;      // ~30 FPS
constexpr uint32_t kTickIntervalMs = 5000; // 5 sec game tick
constexpr uint32_t kAutosaveIntervalMs = 30000;

// Buttons GPIO (TODO: verify on target PCB)
constexpr int kPinButtonUp = 4;
constexpr int kPinButtonAction = 5;
constexpr int kPinButtonDown = 6;

// Output GPIO (TODO: verify on target PCB)
constexpr int kPinBuzzer = 7;
constexpr int kPinVibration = 8;

// ST7789 pins (TODO: verify wiring)
constexpr int kTftCs = 10;
constexpr int kTftDc = 9;
constexpr int kTftRst = 12;
constexpr int kTftWidth = 240;
constexpr int kTftHeight = 280;

constexpr uint32_t kDebounceMs = 50;
constexpr uint32_t kHoldSuppressMs = 3000;
constexpr uint32_t kHoldInfoComboMs = 2000;

}  // namespace xenovent::config
