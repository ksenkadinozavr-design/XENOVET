#include "domain/types.h"

#include <algorithm>

#include "config/constants.h"

namespace xenovent::domain {

int clampStat(int value) {
  return std::clamp(value, config::kStatMin, config::kStatMax);
}

void applyTick(CreatureState& state, const TickContext& ctx) {
  const int dt = static_cast<int>(ctx.dtSeconds);
  state.ageSeconds += ctx.dtSeconds;

  state.hunger = clampStat(state.hunger + (1 * dt));
  state.essence = clampStat(state.essence - (1 * dt));
  state.instability = clampStat(state.instability + (state.hunger > 70 ? 2 : 1) * dt);

  if (state.bond > 50) {
    state.instability = clampStat(state.instability - dt);
  }

  if (state.corruption > 60) {
    state.essence = clampStat(state.essence - dt);
  }

  if (ctx.sensors.validLight && ctx.sensors.lightLux > 500.0f) {
    state.essence = clampStat(state.essence + dt);
  }

  if (ctx.sensors.validAccel && ctx.sensors.accelMagnitude > 15.0f) {
    state.instability = clampStat(state.instability + dt);
  }

  if (state.hunger > 90) {
    state.essence = clampStat(state.essence - (2 * dt));
  }
}

UiFlags deriveUiFlags(const CreatureState& state) {
  UiFlags flags;
  flags.starving = state.hunger >= 75;
  flags.unstable = state.instability >= 70;
  flags.highCorruption = state.corruption >= 70;
  flags.bonded = state.bond >= 65;
  flags.nearDeath = (state.essence <= 10) || (state.instability >= 95) || (state.corruption >= 95);
  return flags;
}

bool shouldDie(const CreatureState& state) {
  return state.essence <= 0 || state.instability >= 100 || state.corruption >= 100;
}

void resetAfterDeath(CreatureState& state) {
  state.deaths += 1;
  state.ageSeconds = 0;
  state.essence = 50;
  state.hunger = 30;
  state.instability = 20;
  state.bond = 40;
  state.corruption = 15;
  state.form = CreatureForm::Seed;
}

const char* toString(CreatureForm form) {
  switch (form) {
    case CreatureForm::Seed:
      return "Seed";
    case CreatureForm::Wisp:
      return "Wisp";
    case CreatureForm::Shade:
      return "Shade";
    case CreatureForm::Ascendant:
      return "Ascendant";
  }
  return "Unknown";
}

}  // namespace xenovent::domain
