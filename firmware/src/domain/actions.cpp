#include "domain/types.h"

#include "config/game_balance.h"

namespace xenovent::domain {

namespace {
bool canPerform(const CreatureState& state, ActionType action) {
  switch (action) {
    case ActionType::Feed:
      return state.hunger > 5;
    case ActionType::Suppress:
      return state.instability > 10;
    case ActionType::Ritual:
      return state.essence >= 20;
    case ActionType::Meditate:
      return state.instability >= 10;
    case ActionType::Sleep:
      return state.sleepTicksRemaining == 0;
    case ActionType::None:
      return false;
  }
  return false;
}
}  // namespace

ActionResult applyAction(const CreatureState& state, ActionType action) {
  ActionResult result;
  result.action = action;
  result.before = state;
  result.after = state;

  if (!canPerform(state, action)) {
    result.accepted = false;
    result.message = "precondition_failed";
    return result;
  }

  result.accepted = true;

  switch (action) {
    case ActionType::Feed:
      result.after.hunger -= 24;
      result.after.essence += 8;
      if (state.corruption > 60) {
        result.after.instability += 3;
      }
      if (state.hunger < 20) {
        result.after.corruption += 2;  // overfeed side effect
      }
      result.message = "fed";
      break;
    case ActionType::Suppress:
      result.after.instability -= 20;
      result.after.bond -= 6;
      result.after.corruption += 4;
      result.after.suppressTicksRemaining = config::balance::kSuppressDurationTicks;
      result.message = "suppressed";
      break;
    case ActionType::Ritual:
      result.after.essence += 16;
      result.after.corruption += 8;
      result.after.hunger += 6;
      result.after.instability += 2;
      result.message = "ritual_cast";
      break;
    case ActionType::Meditate:
      result.after.instability -= 14;
      result.after.corruption -= 4;
      result.after.essence += 4;
      if (state.hunger > 80) {
        result.after.essence -= 3;  // weak focus when starving
      }
      result.message = "meditated";
      break;
    case ActionType::Sleep:
      result.after.sleepTicksRemaining = config::balance::kSleepDurationTicks;
      result.after.essence += 6;
      result.after.instability -= 6;
      result.after.hunger += 5;
      result.message = "sleep_started";
      break;
    case ActionType::None:
      result.accepted = false;
      result.message = "none";
      break;
  }

  result.after = normalizeState(result.after);
  return result;
}

const char* toString(ActionType action) {
  switch (action) {
    case ActionType::Feed:
      return "FEED";
    case ActionType::Suppress:
      return "SUPPRESS";
    case ActionType::Ritual:
      return "RITUAL";
    case ActionType::Meditate:
      return "MEDITATE";
    case ActionType::Sleep:
      return "SLEEP";
    case ActionType::None:
      return "NONE";
  }
  return "NONE";
}

}  // namespace xenovent::domain
