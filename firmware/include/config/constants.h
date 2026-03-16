#pragma once

#include "config/app_config.h"
#include "config/game_balance.h"
#include "config/pins.h"
#include "config/ui_config.h"

namespace xenovent::config {

// Backward-compatible aliases for existing call sites.
constexpr int kStatMin = balance::kStatMin;
constexpr int kStatMax = balance::kStatMax;

constexpr uint32_t kFastLoopMs = ui::kFastLoopMs;
constexpr uint32_t kTickIntervalMs = balance::kTickIntervalMs;
constexpr uint32_t kAutosaveIntervalMs = balance::kAutosaveIntervalMs;

constexpr int kPinButtonUp = pins::kButtonUp;
constexpr int kPinButtonAction = pins::kButtonAction;
constexpr int kPinButtonDown = pins::kButtonDown;

constexpr int kPinBuzzer = pins::kBuzzer;
constexpr int kPinVibration = pins::kVibration;

constexpr int kTftCs = pins::kTftCs;
constexpr int kTftDc = pins::kTftDc;
constexpr int kTftRst = pins::kTftRst;
constexpr int kTftWidth = ui::kTftWidth;
constexpr int kTftHeight = ui::kTftHeight;

constexpr uint32_t kDebounceMs = ui::kDebounceMs;
constexpr uint32_t kHoldSuppressMs = ui::kHoldSuppressMs;
constexpr uint32_t kHoldInfoComboMs = ui::kHoldInfoComboMs;

}  // namespace xenovent::config
