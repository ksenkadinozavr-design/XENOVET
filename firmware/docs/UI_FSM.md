# UI_FSM

## UI model
`UiModel` включает:
- текущий экран,
- выбранное действие,
- transient message,
- suppress mode,
- timestamp последнего взаимодействия.

## События
- `ShortPress`
- `LongPress` (ACTION)
- `ComboUpDownHold`

## Правила переходов
- Main + UP -> StatusMenu
- Main + DOWN -> ActionMenu
- ActionMenu + UP/DOWN -> смена выбранного action
- ActionMenu + ACTION -> trigger selected action + Main
- Любой экран + ACTION long -> Suppress + trigger SUPPRESS
- Любой экран + UP+DOWN hold -> InfoScreen
- Status/Info/Suppress + ACTION -> Main
- Illegal transitions помечаются `illegalTransition=true`

## Timeout
`UiFsm::tick()` выполняет auto-return в Main для меню после `kMenuAutoReturnMs`.
