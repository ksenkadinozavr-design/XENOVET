#pragma once

#include "domain/types.h"

namespace xenovent::game {

void processTick(xenovent::domain::CreatureState& state, const xenovent::domain::TickContext& ctx);

}  // namespace xenovent::game
