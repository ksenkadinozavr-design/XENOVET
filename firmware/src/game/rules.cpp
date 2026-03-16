#include "game/rules.h"

#include "game/mutations.h"

namespace xenovent::game {

void processTick(xenovent::domain::CreatureState& state,
                 const xenovent::domain::TickContext& ctx) {
  xenovent::domain::applyTick(state, ctx);

  if (xenovent::domain::shouldDie(state)) {
    xenovent::domain::resetAfterDeath(state);
    return;
  }

  checkMutation(state);
}

}  // namespace xenovent::game
