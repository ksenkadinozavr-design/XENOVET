#pragma once

#include <array>
#include <cstddef>

#include "telemetry/events.h"

namespace xenovent::telemetry {

class EventRecorder {
 public:
  void onEvent(const Event& event);
  std::size_t size() const { return size_; }
  const Event& at(std::size_t index) const { return events_[index]; }
  void clear();

 private:
  std::array<Event, 256> events_{};
  std::size_t size_ = 0;
};

EventRecorder& globalRecorder();
void globalRecorderSink(const Event& event);

}  // namespace xenovent::telemetry
