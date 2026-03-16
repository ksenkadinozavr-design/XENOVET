# HOWTO_BUILD

## 1) Установка
- VS Code + PlatformIO IDE
- или PlatformIO CLI

Проверка:
```bash
pio --version
```

## 2) Сборка
```bash
cd firmware
pio run
```

## 3) Сборка конкретного env
```bash
cd firmware
pio run -e esp32-s3-devkitc-1
```

## Типовые ошибки
- `pio: command not found` -> установить PlatformIO CLI.
- Ошибки библиотек -> повторить `pio run` (скачает deps).
- Ошибки пинов/периферии -> проверить `include/config/pins.h`.
