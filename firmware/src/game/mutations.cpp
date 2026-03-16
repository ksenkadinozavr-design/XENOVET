#include "game/mutations.h"

namespace xenovent::game {

domain::MutationResult evaluateMutation(const domain::CreatureState& state) {
  return domain::checkMutation(state);
}

}  // namespace xenovent::game
