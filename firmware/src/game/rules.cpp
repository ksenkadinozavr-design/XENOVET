#include "game/rules.h"

#include "game/mutations.h"

namespace xenovent::game {

TickResult processTick(const domain::CreatureState& state,
                       const domain::TickContext& ctx) {
  TickResult result;

  // Stage 0: guard input state.
  const domain::CreatureState guardedInput = domain::normalizeState(state);
  result.inputWasNormalized = !domain::isValidState(state) || !(guardedInput.essence == state.essence && guardedInput.hunger == state.hunger && guardedInput.instability == state.instability && guardedInput.bond == state.bond && guardedInput.corruption == state.corruption && guardedInput.form == state.form && guardedInput.sleepTicksRemaining == state.sleepTicksRemaining && guardedInput.suppressTicksRemaining == state.suppressTicksRemaining);

  // Stage 1: base + sensor + form modifiers + effects.
  result.state = domain::applyTick(guardedInput, ctx);

  // Stage 2: derived warnings.
  result.flags = domain::deriveUiFlags(result.state);

  // Stage 3: critical/death.
  result.death = domain::shouldDie(result.state);
  if (result.death.died) {
    result.state = domain::resetAfterDeath(result.state);
    result.flags = domain::deriveUiFlags(result.state);
    result.recommendSave = true;
    result.outputValid = domain::isValidState(result.state);
    result.summary = "death_reset";
    return result;
  }

  // Stage 4: mutation.
  result.mutation = evaluateMutation(result.state);
  if (result.mutation.mutated) {
    result.state.form = result.mutation.to;
    result.recommendSave = true;
    result.summary = "mutation";
  } else {
    result.recommendSave = true;
    result.summary = "tick_update";
  }

  // Stage 5: final output validation.
  result.state = domain::normalizeState(result.state);
  result.outputValid = domain::isValidState(result.state);
  return result;
}

}  // namespace xenovent::game
