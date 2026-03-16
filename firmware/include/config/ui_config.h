#pragma once

#include <cstdint>

namespace xenovent::config::ui {

constexpr uint32_t kFastLoopMs = 33;
constexpr uint32_t kMenuAutoReturnMs = 15000;

constexpr int kTftWidth = 240;
constexpr int kTftHeight = 280;

constexpr uint32_t kDebounceMs = 50;
constexpr uint32_t kHoldSuppressMs = 3000;
constexpr uint32_t kHoldInfoComboMs = 2000;

}  // namespace xenovent::config::ui
