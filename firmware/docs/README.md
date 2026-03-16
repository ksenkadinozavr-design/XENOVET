# XENOVENT Firmware Docs

## Что это
Firmware для игрового устройства XENOVENT с модульной архитектурой:
- domain (чистая логика),
- game/application orchestration,
- ui/fsm/render,
- input/sensors/output adapters,
- storage (NVS/Preferences).

## Стек
- ESP32-S3
- Arduino framework
- PlatformIO
- C++17

## Структура проекта
```text
firmware/
  include/
    config/        # pins, balance, ui/app config
    domain/        # types/actions/pure rules
    game/          # tick pipeline and engine contracts
    ui/            # ui model, fsm, renderer contracts
    input/         # input events + interpreter
    drivers/       # sensor managers
    storage/       # state persistence
    output/        # feedback patterns
    utils/         # logging
  src/
  test/
  docs/
```

## Основные команды
```bash
cd firmware
pio run
pio test
pio run -t upload
pio device monitor -b 115200
```

## Навигация по документации
- Архитектура: `ARCHITECTURE.md`
- Игровые правила/баланс: `GAME_LOGIC.md`
- UI transitions: `UI_FSM.md`
- Хранение состояния: `STORAGE.md`
- Тесты и покрытие: `TESTING.md`
- Сборка: `HOWTO_BUILD.md`
- Прошивка: `HOWTO_FLASH.md`
- Запуск/ручная проверка: `HOWTO_RUN.md`
- Отладка: `HOWTO_DEBUG.md`
- Эмуляция: `HOWTO_SIMULATE.md`
