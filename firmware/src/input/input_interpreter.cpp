#include "input/input_interpreter.h"

#include <cstddef>

#include "config/ui_config.h"

namespace xenovent::input {

namespace {
std::size_t idx(ButtonId id) {
  switch (id) {
    case ButtonId::Up:
      return 0;
    case ButtonId::Action:
      return 1;
    case ButtonId::Down:
      return 2;
  }
  return 0;
}
}  // namespace

bool InputInterpreter::emitButtonEvent(ButtonId id, bool rawPressed, uint32_t nowMs,
                                       InputEvent& outEvent) {
  auto& tr = tracks_[idx(id)];

  if (rawPressed != tr.rawPressed) {
    tr.rawPressed = rawPressed;
    tr.lastChangeMs = nowMs;
  }

  if (nowMs - tr.lastChangeMs >= config::ui::kDebounceMs && rawPressed != tr.stablePressed) {
    tr.stablePressed = rawPressed;
    if (tr.stablePressed) {
      tr.pressedSinceMs = nowMs;
      tr.holdFired = false;
    } else if (!tr.holdFired) {
      outEvent = {InputEventType::ShortPress, id, false};
      return true;
    }
  }

  if (id == ButtonId::Action && tr.stablePressed && !tr.holdFired &&
      nowMs - tr.pressedSinceMs >= config::ui::kHoldSuppressMs) {
    tr.holdFired = true;
    outEvent = {InputEventType::LongPress, id, false};
    return true;
  }

  return false;
}

bool InputInterpreter::update(const RawButtonState& raw, uint32_t nowMs, InputEvent& outEvent) {
  if (emitButtonEvent(ButtonId::Up, raw.upPressed, nowMs, outEvent)) return true;
  if (emitButtonEvent(ButtonId::Action, raw.actionPressed, nowMs, outEvent)) return true;
  if (emitButtonEvent(ButtonId::Down, raw.downPressed, nowMs, outEvent)) return true;

  const auto& up = tracks_[idx(ButtonId::Up)];
  const auto& down = tracks_[idx(ButtonId::Down)];
  if (up.stablePressed && down.stablePressed) {
    const uint32_t startMs = up.pressedSinceMs > down.pressedSinceMs ? up.pressedSinceMs : down.pressedSinceMs;
    if (!comboFired_ && nowMs - startMs >= config::ui::kHoldInfoComboMs) {
      comboFired_ = true;
      outEvent = {InputEventType::ComboUpDownHold, ButtonId::Action, false};
      return true;
    }
  } else {
    comboFired_ = false;
  }

  return false;
}

}  // namespace xenovent::input
