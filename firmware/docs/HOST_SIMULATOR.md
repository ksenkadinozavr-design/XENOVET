# HOST_SIMULATOR

## Назначение
Native-режим для быстрого прогона rules/FSM/telemetry без ESP32.

## Build
```bash
cd firmware
pio run -e native
```

## Run
```bash
cd firmware
.pio/build/native/program
```

## Что делает harness
- выполняет scripted input scenario,
- запускает tick processing,
- применяет actions,
- пишет telemetry events в recorder,
- печатает финальный state и первые события.

## Ограничения
- Нет реального ST7789/sensors/Preferences.
- Используется shared business logic из `domain/game/ui/input/telemetry`.
