#include "domain/types.h"

namespace xenovent::domain {

void applyAction(CreatureState& state, ActionType action) {
  switch (action) {
    case ActionType::Feed:
      state.hunger = clampStat(state.hunger - 20);
      state.essence = clampStat(state.essence + 10);
      state.corruption = clampStat(state.corruption + 1);
      break;
    case ActionType::Suppress:
      state.instability = clampStat(state.instability - 25);
      state.bond = clampStat(state.bond - 5);
      state.corruption = clampStat(state.corruption + 5);
      break;
    case ActionType::Ritual:
      state.essence = clampStat(state.essence + 15);
      state.corruption = clampStat(state.corruption + 10);
      state.bond = clampStat(state.bond + 2);
      break;
    case ActionType::Meditate:
      state.instability = clampStat(state.instability - 15);
      state.corruption = clampStat(state.corruption - 5);
      state.essence = clampStat(state.essence + 5);
      break;
    case ActionType::Sleep:
      state.essence = clampStat(state.essence + 8);
      state.hunger = clampStat(state.hunger + 5);
      state.instability = clampStat(state.instability - 10);
      break;
    case ActionType::None:
      break;
  }
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
