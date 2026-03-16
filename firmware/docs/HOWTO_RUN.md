# HOWTO_RUN

## После старта
- Инициализируются logger, storage, input/sensors/output/renderer.
- Загружается state из NVS или default fallback.
- Запускаются fast loop + tick loop + autosave policy.

## Контролы
- UP short -> StatusMenu
- DOWN short -> ActionMenu
- ACTION short -> confirm/back
- ACTION long -> Suppress action
- UP+DOWN hold -> InfoScreen

## Ручной smoke scenario
1. Откройте ActionMenu.
2. Выберите FEED и подтвердите.
3. Убедитесь, что показан transient message.
4. Подождите минимум один tick.
5. Проверьте, что статы изменились и autosave отработал позже.

## Без железа
- Логика: `pio test`
- Эмуляция: `HOWTO_SIMULATE.md`
