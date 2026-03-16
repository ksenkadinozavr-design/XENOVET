# TESTING

## Запуск
```bash
cd firmware
pio test
```

## Что покрыто
- Domain:
  - clamp/normalize,
  - applyTick,
  - applyAction для каждого action,
  - shouldDie/reset,
  - mutation transitions.
- UI FSM:
  - базовые переходы,
  - illegal transition,
  - combo event,
  - timeout auto-return.
- Input interpreter:
  - ACTION long press,
  - UP+DOWN hold combo.

## Edge-cases
Проверяются границы 0/100 и предикаты отклонения действий через `ActionResult.accepted`.

## Что ещё добавить дальше
- table-driven tests для form-specific modifiers,
- fuzz-like checks для последовательностей input events,
- persistence integration tests на железе.
