#pragma once

#include <array>
#include <cstddef>

#include "telemetry/events.h"

namespace xenovent::telemetry {

class EventBus {
 public:
  using Handler = void (*)(const Event& event);

  bool subscribe(Handler handler);
  void publish(const Event& event) const;

 private:
  std::array<Handler, 12> handlers_{};
};

}  // namespace xenovent::telemetry
