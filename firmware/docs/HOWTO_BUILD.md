# HOWTO_BUILD

## 1. Установка
1. Установите VS Code.
2. Установите расширение PlatformIO IDE.
3. Проверьте доступность CLI:
```bash
pio --version
```

## 2. Открытие проекта
Открывайте папку `firmware` как корень PlatformIO проекта.

## 3. Сборка
Из папки `firmware`:
```bash
cd firmware
pio run
```

## 4. Выбор environment
По умолчанию: `esp32-s3-devkitc-1`.
Явно:
```bash
pio run -e esp32-s3-devkitc-1
```

## 5. Типовые ошибки
- Не скачались библиотеки: повторите `pio run`.
- Неверный board: проверьте `platformio.ini`.
- Ошибка пинов/дисплея: проверьте `include/config/constants.h` (TODO значения).
