# HOWTO_RUN

## После запуска
- Загружается state из NVS.
- Инициализируются кнопки, сенсоры, output, renderer.
- Запускаются fast loop и tick loop.

## Управление кнопками
- UP short -> StatusMenu
- DOWN short -> ActionMenu
- ACTION short -> back/confirm
- UP+DOWN hold 2s -> InfoScreen
- ACTION hold 3s -> Suppress action

## Проверка экранов
1. Перейдите в StatusMenu/ActionMenu.
2. В ActionMenu пролистывайте UP/DOWN и подтвердите ACTION.
3. Убедитесь, что после confirm возврат на Main.

## Проверка тиков
Подождите >=5 секунд: должны обновляться статы (hunger/essence и др.).

## Проверка save/load
1. Сделайте действие (например FEED).
2. Подождите autosave (~30s).
3. Перезагрузите устройство.
4. Убедитесь, что state восстановился.

## Проверка сенсоров
- При ярком свете должен расти ESSENCE.
- При резком движении должен расти INSTABILITY.

## Сценарий первого запуска
1. Прошить устройство.
2. Открыть монитор.
3. Проверить Main экран.
4. Открыть ActionMenu, выполнить FEED.
5. Дождаться тик-обновления и autosave.
