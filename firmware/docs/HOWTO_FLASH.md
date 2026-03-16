# HOWTO_FLASH

## Подключение
```bash
pio device list
```

## Прошивка
```bash
cd firmware
pio run -t upload
```

С явным портом:
```bash
cd firmware
pio run -t upload --upload-port /dev/ttyACM0
```

## Монитор
```bash
cd firmware
pio device monitor -b 115200
```
