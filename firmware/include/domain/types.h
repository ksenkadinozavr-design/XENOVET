#pragma once

#include <cstdint>

#include "domain/actions.h"

namespace xenovent::domain {

enum class CreatureForm : uint8_t {
  Seed = 0,
  Wisp,
  Sentinel,
  Shade,
  Ascendant,
  Ruinborn,
};

enum class UiScreen : uint8_t {
  Main = 0,
  StatusMenu,
  ActionMenu,
  InfoScreen,
  Suppress,
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

  // Persisted gameplay effects.
  uint32_t sleepTicksRemaining = 0;
  uint32_t suppressTicksRemaining = 0;
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
  bool ritualResidual = false;
};

struct UiFlags {
  bool starving = false;
  bool unstable = false;
  bool highCorruption = false;
  bool bonded = false;
  bool nearDeath = false;
  bool sleeping = false;
  bool suppressed = false;
};

struct MutationResult {
  bool mutated = false;
  CreatureForm from = CreatureForm::Seed;
  CreatureForm to = CreatureForm::Seed;
  const char* reason = "none";
};

struct ActionResult {
  bool accepted = false;
  ActionType action = ActionType::None;
  CreatureState before{};
  CreatureState after{};
  const char* message = "ignored";
};

struct DeathResult {
  bool died = false;
  const char* reason = "alive";
};

int clampStat(int value);
CreatureState buildDefaultState();
CreatureState normalizeState(const CreatureState& state);
bool isValidState(const CreatureState& state);

int maybeDecayBond(const CreatureState& state, const TickContext& ctx);
int maybeIncreaseCorruption(const CreatureState& state, const TickContext& ctx);
int maybeRecoverEssence(const CreatureState& state, const TickContext& ctx);

CreatureState applyTick(const CreatureState& state, const TickContext& ctx);
ActionResult applyAction(const CreatureState& state, ActionType action);
UiFlags deriveUiFlags(const CreatureState& state);
MutationResult checkMutation(const CreatureState& state);
DeathResult shouldDie(const CreatureState& state);
CreatureState resetAfterDeath(const CreatureState& state);

const char* toString(CreatureForm form);
const char* toString(ActionType action);
const char* toString(UiScreen screen);

}  // namespace xenovent::domain
