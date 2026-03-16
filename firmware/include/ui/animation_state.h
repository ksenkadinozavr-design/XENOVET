#pragma once

#include <cstdint>

namespace xenovent::ui {

struct AnimationState {
  uint8_t pulse = 0;
  bool blinkOn = false;
};

AnimationState buildAnimationState(uint32_t nowMs);

}  // namespace xenovent::ui
