#include "game/rules.h"

#include "game/mutations.h"

namespace xenovent::game {

TickResult processTick(const domain::CreatureState& state,
                       const domain::TickContext& ctx) {
  TickResult result;

  // 1) base + sensor + form modifiers + effects.
  result.state = domain::applyTick(state, ctx);

  // 2) derived warnings.
  result.flags = domain::deriveUiFlags(result.state);

  // 3) critical/death.
  result.death = domain::shouldDie(result.state);
  if (result.death.died) {
    result.state = domain::resetAfterDeath(result.state);
    result.flags = domain::deriveUiFlags(result.state);
    result.recommendSave = true;
    result.summary = "death_reset";
    return result;
  }

  // 4) mutation.
  result.mutation = evaluateMutation(result.state);
  if (result.mutation.mutated) {
    result.state.form = result.mutation.to;
    result.recommendSave = true;
    result.summary = "mutation";
  } else {
    result.recommendSave = true;
    result.summary = "tick_update";
  }

  return result;
}

}  // namespace xenovent::game
