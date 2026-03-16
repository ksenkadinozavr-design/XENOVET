#include "telemetry/recorder.h"

namespace xenovent::telemetry {

void EventRecorder::onEvent(const Event& event) {
  if (size_ < events_.size()) {
    events_[size_] = event;
    size_ += 1;
    return;
  }
  for (std::size_t i = 1; i < events_.size(); ++i) {
    events_[i - 1] = events_[i];
  }
  events_[events_.size() - 1] = event;
}

void EventRecorder::clear() { size_ = 0; }

EventRecorder& globalRecorder() {
  static EventRecorder recorder;
  return recorder;
}

void globalRecorderSink(const Event& event) { globalRecorder().onEvent(event); }

}  // namespace xenovent::telemetry
