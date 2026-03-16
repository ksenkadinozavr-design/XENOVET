#pragma once

#include "domain/types.h"

namespace xenovent::game {

struct TickResult {
  domain::CreatureState state{};
  domain::UiFlags flags{};
  domain::MutationResult mutation{};
  domain::DeathResult death{};
  bool recommendSave = false;
  const char* summary = "tick";
};

TickResult processTick(const domain::CreatureState& state, const domain::TickContext& ctx);

}  // namespace xenovent::game
