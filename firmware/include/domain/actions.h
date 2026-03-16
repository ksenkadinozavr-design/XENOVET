#pragma once

#include <cstdint>

namespace xenovent::domain {

enum class ActionType : uint8_t {
  Feed = 0,
  Suppress,
  Ritual,
  Meditate,
  Sleep,
  None,
};

}  // namespace xenovent::domain
