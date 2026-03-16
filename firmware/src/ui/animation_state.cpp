#include "ui/animation_state.h"

namespace xenovent::ui {

AnimationState buildAnimationState(uint32_t nowMs) {
  AnimationState st;
  st.pulse = static_cast<uint8_t>((nowMs / 120) % 6);
  st.blinkOn = ((nowMs / 400) % 2) == 0;
  return st;
}

}  // namespace xenovent::ui
