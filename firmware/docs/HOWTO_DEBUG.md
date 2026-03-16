# HOWTO_DEBUG

## Serial debug
Запустить монитор:
```bash
cd firmware
pio device monitor -b 115200
```

## Какие логи включены
- Renderer fallback лог (если ST7789 init failed).
- ADXL/BH1750 init failed.
- UI state dump в serial (fallback mode).

## Типовые проблемы

### Дисплей пустой
- Проверьте пины CS/DC/RST в `include/config/constants.h`.
- Проверьте питание дисплея и SPI wiring.
- Временно работайте через serial fallback.

### ADXL345 не отвечает
- Проверить I2C wiring SDA/SCL.
- Проверить адрес/подключение питания.
- Сверить лог `[ADXL345] init failed`.

### BH1750 не отвечает
- Проверить I2C wiring.
- Проверить, не конфликтует ли адрес на шине.
- Сверить лог `[BH1750] init failed`.

### Кнопки работают неправильно
- Проверить active-low wiring и `INPUT_PULLUP`.
- Проверить GPIO в constants.
- Проверить debounce/hold тайминги в constants.

### Состояние не сохраняется
- Убедиться, что прошло autosave окно.
- Проверить, вызывается ли `saveState`.
- При необходимости вызвать factory reset и проверить повторно.
