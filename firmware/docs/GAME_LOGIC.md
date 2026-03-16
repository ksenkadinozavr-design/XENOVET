# GAME_LOGIC

## Статы
- ESSENCE: жизненная энергия.
- HUNGER: голод (рост ухудшает другие статы).
- INSTABILITY: психо/энергетическая нестабильность.
- BOND: связь с пользователем.
- CORRUPTION: степень искажения.

Все статы нормализуются в диапазон `0..100` через `normalizeState()`.

## Tick pipeline
`game::processTick()`:
1. `applyTick()` — time decay, sensor effects, form modifiers, lingering effects.
2. `deriveUiFlags()` — warnings для UI.
3. `shouldDie()` — критические условия.
4. `resetAfterDeath()` при смерти.
5. `checkMutation()` и применение новой формы.
6. Формирование `TickResult`.

## Действия
`applyAction()` возвращает `ActionResult`:
- FEED: уменьшает hunger, повышает essence, имеет риск corruption/instability.
- SUPPRESS: резко снижает instability, но уменьшает bond и увеличивает corruption.
- RITUAL: boost essence с побочками на corruption/hunger.
- MEDITATE: стабилизирует instability/corruption, но слабее при сильном hunger.
- SLEEP: включает несколько sleep ticks и мягкое восстановление.

## Мутации
`checkMutation()` формирует `MutationResult`.
Примеры:
- corruption+instability -> Shade,
- bond+stability -> Sentinel,
- high essence+bond+age+low corruption -> Ascendant,
- frequent deaths + high corruption -> Ruinborn.

## Смерть/сброс
`shouldDie()` возвращает `DeathResult` с причиной (`essence|instability|corruption`).
`resetAfterDeath()` возвращает normalized default state и увеличивает `deaths`.
