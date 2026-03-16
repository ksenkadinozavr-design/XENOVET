#pragma once

#include <cstdint>

#include "domain/types.h"

namespace xenovent::ui {

struct IconPrimitive {
  enum class Type : uint8_t { Circle, Rect, Triangle } type = Type::Circle;
  int size = 8;
};

IconPrimitive iconForForm(domain::CreatureForm form);

}  // namespace xenovent::ui
