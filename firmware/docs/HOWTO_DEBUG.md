# HOWTO_DEBUG

## Serial logs
```bash
cd firmware
pio device monitor -b 115200
```

## Что логируется
- startup sequence,
- загрузка/сохранение state,
- action accepted/rejected,
- death/mutation события,
- sensor init health.

## Если дисплей пустой
- проверить `config/pins.h` для ST7789,
- проверить питание и SPI wiring,
- смотреть serial logs renderer.

## Если кнопки работают неверно
- проверить active-low wiring,
- проверить debounce/hold параметры `config/ui_config.h`,
- проверить input events через debug logs.

## Если сенсоры не отвечают
- проверить I2C wiring/адреса,
- убедиться, что fallback логирует degraded mode.

## Если не сохраняется state
- проверить autosave interval,
- проверить version mismatch warning,
- вызвать `resetState()` и повторить тест.
