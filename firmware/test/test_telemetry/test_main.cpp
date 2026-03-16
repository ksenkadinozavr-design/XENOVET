#include <unity.h>

#include "telemetry/event_bus.h"
#include "telemetry/recorder.h"

using namespace xenovent;

void test_event_bus_publish_subscribe() {
  telemetry::EventBus bus;
  telemetry::globalRecorder().clear();
  TEST_ASSERT_TRUE(bus.subscribe(telemetry::globalRecorderSink));

  telemetry::Event ev;
  ev.type = telemetry::EventType::TickProcessed;
  ev.timestampMs = 42;
  ev.valueA = 1;
  ev.valueB = 2;
  ev.message = "tick";
  bus.publish(ev);

  TEST_ASSERT_EQUAL(1, telemetry::globalRecorder().size());
  const auto& rec = telemetry::globalRecorder().at(0);
  TEST_ASSERT_EQUAL(telemetry::EventType::TickProcessed, rec.type);
  TEST_ASSERT_EQUAL(42, rec.timestampMs);
}

void setup() {
  UNITY_BEGIN();
  RUN_TEST(test_event_bus_publish_subscribe);
  UNITY_END();
}

void loop() {}
