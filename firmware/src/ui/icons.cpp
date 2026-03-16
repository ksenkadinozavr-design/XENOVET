#include "ui/icons.h"

namespace xenovent::ui {

IconPrimitive iconForForm(domain::CreatureForm form) {
  switch (form) {
    case domain::CreatureForm::Seed:
      return {IconPrimitive::Type::Circle, 7};
    case domain::CreatureForm::Wisp:
      return {IconPrimitive::Type::Circle, 10};
    case domain::CreatureForm::Sentinel:
      return {IconPrimitive::Type::Rect, 10};
    case domain::CreatureForm::Shade:
      return {IconPrimitive::Type::Triangle, 10};
    case domain::CreatureForm::Ascendant:
      return {IconPrimitive::Type::Circle, 12};
    case domain::CreatureForm::Ruinborn:
      return {IconPrimitive::Type::Rect, 12};
  }
  return {IconPrimitive::Type::Circle, 8};
}

}  // namespace xenovent::ui
