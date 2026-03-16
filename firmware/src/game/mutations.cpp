#include "game/mutations.h"

namespace xenovent::game {

bool checkMutation(xenovent::domain::CreatureState& state) {
  using xenovent::domain::CreatureForm;

  const CreatureForm prev = state.form;

  if (state.essence > 70 && state.bond > 60 && state.corruption < 40) {
    state.form = CreatureForm::Wisp;
  }
  if (state.corruption > 70 && state.instability > 70) {
    state.form = CreatureForm::Shade;
  }
  if (state.essence > 85 && state.bond > 80 && state.corruption < 25 && state.ageSeconds > 300) {
    state.form = CreatureForm::Ascendant;
  }

  return prev != state.form;
}

}  // namespace xenovent::game
