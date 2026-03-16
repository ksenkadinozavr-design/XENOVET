#pragma once

namespace xenovent::config::pins {

// Buttons GPIO (TODO: verify on target PCB)
constexpr int kButtonUp = 4;
constexpr int kButtonAction = 5;
constexpr int kButtonDown = 6;

// Output GPIO (TODO: verify on target PCB)
constexpr int kBuzzer = 7;
constexpr int kVibration = 8;

// ST7789 SPI pins (TODO: verify wiring)
constexpr int kTftCs = 10;
constexpr int kTftDc = 9;
constexpr int kTftRst = 12;

}  // namespace xenovent::config::pins
