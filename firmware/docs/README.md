# XENOVENT Firmware (ESP32-S3, PlatformIO, Arduino)

Firmware MVP для портативного устройства XENOVENT.

## Что реализовано
- Domain model сущности Voidborn и игровой state.
- Tick-based game engine с fast loop + tick loop.
- Правила деградации/взаимодействия статов, смерть/перерождение, мутации.
- UI FSM экранов: Main, StatusMenu, ActionMenu, InfoScreen, Suppress.
- Input manager (short press, hold, UP+DOWN combo, debounce).
- Sensor managers: ADXL345, BH1750.
- Storage manager: Preferences/NVS (save/load/factory reset).
- Output manager: buzzer/vibration abstraction.
- Unit tests для domain/FSM.

## Стек
- MCU: ESP32-S3
- Framework: Arduino
- Build: PlatformIO
- Язык: C++17

## Структура
См. `ARCHITECTURE.md`.

## Быстрый старт
```bash
cd firmware
pio run
pio test
pio run -t upload
pio device monitor
```

## Расширение проекта
- Добавляйте новые действия в `domain/actions.h` + `domain/actions.cpp`.
- Добавляйте мутации в `game/mutations.cpp`.
- Добавляйте UI-экраны через `ui/ui_state.h` и `ui/ui_fsm.cpp`.
- Добавляйте сенсорные источники через `drivers/*` + интеграцию в `game_engine.cpp`.


## Эмуляция
- См. `HOWTO_SIMULATE.md` для запуска в Wokwi и ограничений эмуляции.
