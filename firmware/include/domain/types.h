#pragma once

#include <cstdint>

#include "domain/actions.h"

namespace xenovent::domain {

enum class CreatureForm {
  Seed,
  Wisp,
  Shade,
  Ascendant,
};

struct CreatureState {
  int essence = 60;
  int hunger = 40;
  int instability = 20;
  int bond = 50;
  int corruption = 10;

  CreatureForm form = CreatureForm::Seed;
  uint32_t ageSeconds = 0;
  uint32_t deaths = 0;
};

struct SensorSnapshot {
  float accelMagnitude = 0.0f;
  float lightLux = 0.0f;
  bool validAccel = false;
  bool validLight = false;
};

struct TickContext {
  uint32_t dtSeconds = 0;
  SensorSnapshot sensors{};
};

struct UiFlags {
  bool starving = false;
  bool unstable = false;
  bool highCorruption = false;
  bool bonded = false;
  bool nearDeath = false;
};

int clampStat(int value);
void applyTick(CreatureState& state, const TickContext& ctx);
void applyAction(CreatureState& state, ActionType action);
UiFlags deriveUiFlags(const CreatureState& state);
bool shouldDie(const CreatureState& state);
void resetAfterDeath(CreatureState& state);

const char* toString(CreatureForm form);
const char* toString(ActionType action);

}  // namespace xenovent::domain
