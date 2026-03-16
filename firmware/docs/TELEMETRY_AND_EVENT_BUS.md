# TELEMETRY_AND_EVENT_BUS

## Архитектура
- `telemetry::EventBus`: pub/sub шина.
- `telemetry::Event`: типизированное событие + payload.
- `telemetry::EventRecorder`: ring-like in-memory recorder (для replay/debug и тестов).

## Ключевые event types
- startup,
- storage load/save/migration/recovery,
- input interpreted,
- UI transition,
- action applied,
- cooldown started/ended,
- tick processed,
- mutation/death,
- warnings,
- render frame.

## Replay-debug
- Recorder сохраняет последовательность событий.
- Host simulator выводит сценарный trace.
- Для расширения можно добавить экспорт в JSON/CSV.
