#include <cstdint>
#include <cstdio>
#include <vector>

#include "domain/types.h"
#include "game/rules.h"
#include "telemetry/event_bus.h"
#include "telemetry/recorder.h"
#include "ui/ui_fsm.h"

using namespace xenovent;

struct ScriptedStep {
  uint32_t atMs;
  input::InputEvent event;
};

int main() {
  telemetry::EventBus bus;
  bus.subscribe(telemetry::globalRecorderSink);

  domain::CreatureState state = domain::buildDefaultState();
  ui::UiFsm fsm;

  std::vector<ScriptedStep> script = {
      {100, {input::InputEventType::ShortPress, input::ButtonId::Down, false}},
      {200, {input::InputEventType::ShortPress, input::ButtonId::Action, false}},
      {1200, {input::InputEventType::LongPress, input::ButtonId::Action, false}},
      {3000, {input::InputEventType::ComboUpDownHold, input::ButtonId::Action, false}},
  };

  uint32_t now = 0;
  uint32_t nextTickMs = 1000;
  size_t idx = 0;

  while (now <= 10000) {
    while (idx < script.size() && script[idx].atMs <= now) {
      auto t = fsm.handleEvent(script[idx].event, now);
      if (t.triggeredAction != domain::ActionType::None) {
        const auto ar = domain::applyAction(state, t.triggeredAction);
        if (ar.accepted) {
          state = ar.after;
          bus.publish({telemetry::EventType::ActionApplied, now,
                       static_cast<int32_t>(t.triggeredAction), 1, ar.message});
        }
      }
      idx++;
    }

    if (now >= nextTickMs) {
      domain::TickContext ctx;
      ctx.dtSeconds = 1;
      auto tick = game::processTick(state, ctx);
      state = tick.state;
      bus.publish({telemetry::EventType::TickProcessed, now, static_cast<int32_t>(state.essence),
                   static_cast<int32_t>(state.hunger), tick.summary});
      nextTickMs += 1000;
    }

    now += 100;
  }

  printf("=== HOST SIM RESULT ===\n");
  printf("Form=%s E=%d H=%d I=%d B=%d C=%d deaths=%lu\n", domain::toString(state.form), state.essence,
         state.hunger, state.instability, state.bond, state.corruption,
         static_cast<unsigned long>(state.deaths));

  auto& rec = telemetry::globalRecorder();
  printf("events=%zu\n", rec.size());
  for (size_t i = 0; i < rec.size() && i < 20; ++i) {
    const auto& e = rec.at(i);
    printf("[%zu] t=%lu type=%d a=%ld b=%ld msg=%s\n", i,
           static_cast<unsigned long>(e.timestampMs), static_cast<int>(e.type),
           static_cast<long>(e.valueA), static_cast<long>(e.valueB), e.message);
  }

  return 0;
}
