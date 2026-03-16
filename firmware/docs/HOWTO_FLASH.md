# HOWTO_FLASH

## 1. Подключение ESP32-S3
1. Подключите плату по USB.
2. Узнайте порт:
```bash
pio device list
```

## 2. Прошивка
Из папки `firmware`:
```bash
cd firmware
pio run -t upload
```
Если нужен явный порт:
```bash
pio run -t upload --upload-port /dev/ttyACM0
```

## 3. Проверка прошивки
После upload откройте serial monitor:
```bash
pio device monitor -b 115200
```
Должны появиться startup-логи и UI-логи (если дисплей не инициализировался).
