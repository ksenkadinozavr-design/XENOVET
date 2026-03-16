# TESTING

## Запуск
```bash
cd firmware
pio test
```

## Что покрыто
- Domain:
  - clamp/normalize,
  - applyTick,
  - applyAction для каждого action,
  - shouldDie/reset,
  - mutation transitions.
- UI FSM:
  - базовые переходы,
  - illegal transition,
  - combo event,
  - timeout auto-return.
- Input interpreter:
  - ACTION long press,
  - UP+DOWN hold combo,
  - debounce/combo-release regressions.
- Migrations:
  - v1->v3 success path,
  - unsupported version fallback path.
- Telemetry:
  - event bus publish/subscribe + recorder sink.
- Cooldowns:
  - cooldown start/decay and action unavailability while cooling down.

## Edge-cases
Проверяются границы 0/100 и предикаты отклонения действий через `ActionResult.accepted`.

## Что ещё добавить дальше
- table-driven tests для form-specific modifiers,
- fuzz-like checks для последовательностей input events,
- persistence integration tests на железе.

## Native host smoke
```bash
cd firmware
g++ -std=c++17 -I include host/main_host.cpp src/domain/types.cpp src/domain/actions.cpp src/game/rules.cpp src/game/mutations.cpp src/input/input_interpreter.cpp src/ui/ui_fsm.cpp src/storage/migrations.cpp src/telemetry/event_bus.cpp src/telemetry/recorder.cpp -o /tmp/xenovent_host
/tmp/xenovent_host
```
