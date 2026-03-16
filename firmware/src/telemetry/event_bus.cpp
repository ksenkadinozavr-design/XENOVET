#include "telemetry/event_bus.h"

namespace xenovent::telemetry {

bool EventBus::subscribe(Handler handler) {
  for (auto& h : handlers_) {
    if (h == nullptr) {
      h = handler;
      return true;
    }
  }
  return false;
}

void EventBus::publish(const Event& event) const {
  for (const auto& h : handlers_) {
    if (h != nullptr) {
      h(event);
    }
  }
}

}  // namespace xenovent::telemetry
