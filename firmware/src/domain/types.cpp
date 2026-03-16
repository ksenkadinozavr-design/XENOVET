#include "domain/types.h"

#include <algorithm>

#include "config/game_balance.h"

namespace xenovent::domain {

namespace {

bool isKnownForm(CreatureForm form) {
  switch (form) {
    case CreatureForm::Seed:
    case CreatureForm::Wisp:
    case CreatureForm::Sentinel:
    case CreatureForm::Shade:
    case CreatureForm::Ascendant:
    case CreatureForm::Ruinborn:
      return true;
  }
  return false;
}

}  // namespace

int clampStat(int value) {
  return std::clamp(value, config::balance::kStatMin, config::balance::kStatMax);
}

CreatureState buildDefaultState() { return CreatureState{}; }

CreatureState normalizeState(const CreatureState& state) {
  CreatureState out = state;
  out.essence = clampStat(out.essence);
  out.hunger = clampStat(out.hunger);
  out.instability = clampStat(out.instability);
  out.bond = clampStat(out.bond);
  out.corruption = clampStat(out.corruption);
  if (!isKnownForm(out.form)) {
    out.form = CreatureForm::Seed;
  }
  if (out.sleepTicksRemaining > 1000) out.sleepTicksRemaining = 1000;
  if (out.suppressTicksRemaining > 1000) out.suppressTicksRemaining = 1000;
  if (out.cooldowns.feed > 1000) out.cooldowns.feed = 1000;
  if (out.cooldowns.suppress > 1000) out.cooldowns.suppress = 1000;
  if (out.cooldowns.ritual > 1000) out.cooldowns.ritual = 1000;
  if (out.cooldowns.meditate > 1000) out.cooldowns.meditate = 1000;
  if (out.cooldowns.sleep > 1000) out.cooldowns.sleep = 1000;
  return out;
}

bool isValidState(const CreatureState& state) {
  const bool statsValid =
      state.essence >= config::balance::kStatMin && state.essence <= config::balance::kStatMax &&
      state.hunger >= config::balance::kStatMin && state.hunger <= config::balance::kStatMax &&
      state.instability >= config::balance::kStatMin && state.instability <= config::balance::kStatMax &&
      state.bond >= config::balance::kStatMin && state.bond <= config::balance::kStatMax &&
      state.corruption >= config::balance::kStatMin && state.corruption <= config::balance::kStatMax;
  return statsValid && isKnownForm(state.form);
}

uint32_t getRemainingCooldown(const CreatureState& state, ActionType action) {
  switch (action) {
    case ActionType::Feed:
      return state.cooldowns.feed;
    case ActionType::Suppress:
      return state.cooldowns.suppress;
    case ActionType::Ritual:
      return state.cooldowns.ritual;
    case ActionType::Meditate:
      return state.cooldowns.meditate;
    case ActionType::Sleep:
      return state.cooldowns.sleep;
    case ActionType::None:
      return 0;
  }
  return 0;
}

bool isActionAvailable(const CreatureState& state, ActionType action) {
  return getRemainingCooldown(state, action) == 0;
}

int maybeDecayBond(const CreatureState& state, const TickContext& ctx) {
  int delta = 0;
  if (state.hunger >= 80) delta -= static_cast<int>(ctx.dtSeconds);
  if (state.corruption >= 70) delta -= static_cast<int>(ctx.dtSeconds);
  return delta;
}

int maybeIncreaseCorruption(const CreatureState& state, const TickContext& ctx) {
  int delta = 0;
  if (state.instability >= 75) delta += static_cast<int>(ctx.dtSeconds);
  if (state.hunger >= 90) delta += static_cast<int>(ctx.dtSeconds);
  if (ctx.ritualResidual) delta += 1;
  return delta;
}

int maybeRecoverEssence(const CreatureState& state, const TickContext& ctx) {
  int delta = 0;
  if (ctx.sensors.validLight && ctx.sensors.lightLux >= config::balance::kLuxRecoverThreshold) {
    delta += static_cast<int>(ctx.dtSeconds);
  }
  if (state.sleepTicksRemaining > 0) {
    delta += static_cast<int>(ctx.dtSeconds);
  }
  return delta;
}

CreatureState applyTick(const CreatureState& state, const TickContext& ctx) {
  CreatureState out = state;
  const int dt = static_cast<int>(ctx.dtSeconds);
  out.ageSeconds += ctx.dtSeconds;

  auto dec = [dt](uint32_t& v) { v = (v > static_cast<uint32_t>(dt)) ? (v - static_cast<uint32_t>(dt)) : 0; };
  dec(out.cooldowns.feed);
  dec(out.cooldowns.suppress);
  dec(out.cooldowns.ritual);
  dec(out.cooldowns.meditate);
  dec(out.cooldowns.sleep);

  out.hunger += config::balance::kHungerGainPerSecond * dt;
  out.essence -= config::balance::kEssenceDecayPerSecond * dt;
  out.instability += config::balance::kInstabilityGainPerSecond * dt;

  out.bond += maybeDecayBond(out, ctx);
  out.corruption += maybeIncreaseCorruption(out, ctx);
  out.essence += maybeRecoverEssence(out, ctx);

  if (ctx.sensors.validAccel &&
      ctx.sensors.accelMagnitude >= config::balance::kShakeInstabilityThreshold) {
    out.instability += dt;
  }

  if (out.suppressTicksRemaining > 0) {
    out.suppressTicksRemaining -= 1;
    out.instability -= 2 * dt;
    out.bond -= dt;
  }

  if (out.sleepTicksRemaining > 0) {
    out.sleepTicksRemaining -= 1;
    out.instability -= dt;
  }

  switch (out.form) {
    case CreatureForm::Wisp:
      out.essence += dt;
      break;
    case CreatureForm::Sentinel:
      out.bond += dt;
      out.instability -= dt;
      break;
    case CreatureForm::Shade:
      out.corruption += dt;
      out.bond -= dt;
      break;
    case CreatureForm::Ascendant:
      out.essence += dt;
      out.corruption -= dt;
      break;
    case CreatureForm::Ruinborn:
      out.corruption += 2 * dt;
      out.essence -= dt;
      break;
    case CreatureForm::Seed:
      break;
  }

  if (out.hunger >= 90) {
    out.essence -= 2 * dt;
  }

  return normalizeState(out);
}

UiFlags deriveUiFlags(const CreatureState& state) {
  UiFlags flags;
  flags.starving = state.hunger >= config::balance::kWarningHunger;
  flags.unstable = state.instability >= config::balance::kWarningInstability;
  flags.highCorruption = state.corruption >= config::balance::kWarningCorruption;
  flags.bonded = state.bond >= config::balance::kWarningBonded;
  flags.nearDeath = state.essence <= 10 || state.instability >= 95 || state.corruption >= 95;
  flags.sleeping = state.sleepTicksRemaining > 0;
  flags.suppressed = state.suppressTicksRemaining > 0;
  return flags;
}

MutationResult checkMutation(const CreatureState& state) {
  MutationResult result;
  result.from = state.form;
  result.to = state.form;

  if (state.deaths >= config::balance::kCorruptorMinDeaths && state.corruption >= 85) {
    result.to = CreatureForm::Ruinborn;
    result.reason = "deaths+corruption";
  } else if (state.corruption >= 75 && state.instability >= 70) {
    result.to = CreatureForm::Shade;
    result.reason = "corruption+instability";
  } else if (state.bond >= 75 && state.instability <= 35 && state.corruption <= 45) {
    result.to = CreatureForm::Sentinel;
    result.reason = "bond+stability";
  } else if (state.essence >= 85 && state.bond >= 80 &&
             state.ageSeconds >= config::balance::kAscendMinAgeSeconds && state.corruption <= 25) {
    result.to = CreatureForm::Ascendant;
    result.reason = "ascension";
  } else if (state.essence >= 70 && state.bond >= 60 && state.corruption < 40) {
    result.to = CreatureForm::Wisp;
    result.reason = "early-growth";
  } else {
    result.reason = "no-rule";
  }

  result.mutated = result.to != result.from;
  return result;
}

DeathResult shouldDie(const CreatureState& state) {
  if (state.essence <= config::balance::kDeathEssence) {
    return {true, "essence"};
  }
  if (state.instability >= config::balance::kDeathInstability) {
    return {true, "instability"};
  }
  if (state.corruption >= config::balance::kDeathCorruption) {
    return {true, "corruption"};
  }
  return {false, "alive"};
}

CreatureState resetAfterDeath(const CreatureState& state) {
  CreatureState out = buildDefaultState();
  out.deaths = state.deaths + 1;
  out.form = CreatureForm::Seed;
  return out;
}

const char* toString(CreatureForm form) {
  switch (form) {
    case CreatureForm::Seed:
      return "Seed";
    case CreatureForm::Wisp:
      return "Wisp";
    case CreatureForm::Sentinel:
      return "Sentinel";
    case CreatureForm::Shade:
      return "Shade";
    case CreatureForm::Ascendant:
      return "Ascendant";
    case CreatureForm::Ruinborn:
      return "Ruinborn";
  }
  return "Unknown";
}

const char* toString(UiScreen screen) {
  switch (screen) {
    case UiScreen::Main:
      return "Main";
    case UiScreen::StatusMenu:
      return "Status";
    case UiScreen::ActionMenu:
      return "Actions";
    case UiScreen::InfoScreen:
      return "Info";
    case UiScreen::Suppress:
      return "Suppress";
  }
  return "Unknown";
}

}  // namespace xenovent::domain
