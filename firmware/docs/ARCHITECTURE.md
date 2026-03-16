# ARCHITECTURE

## Модули
- `domain/`: чистая модель и правила изменения состояния.
- `game/`: orchestration тиков, применение правил, мутаций, автосохранения.
- `ui/`: FSM экранов и renderer.
- `input/`: кнопки, debounce/hold/combo.
- `drivers/`: ADXL345 и BH1750.
- `storage/`: NVS/Preferences.
- `output/`: buzzer/vibration.

## Поток данных
1. `ButtonManager` генерирует `InputEvent`.
2. `UiFsm` решает переходы/действия.
3. `GameEngine` применяет `applyAction`.
4. По tick-интервалу движок читает сенсоры и вызывает `processTick`.
5. `Renderer` рисует текущий UI/state.
6. `StateStorage` автосохраняет dirty-state.

## Игровой цикл
- Fast loop (~30 FPS): input -> fsm -> render.
- Tick loop (5s): sensor snapshot -> applyTick -> shouldDie/reset -> mutation checks.

## FSM переходы
- Main + UP short -> StatusMenu
- Main + DOWN short -> ActionMenu
- ActionMenu + UP/DOWN short -> смена action
- ActionMenu + ACTION short -> apply action + Main
- Status/Info/Suppress + ACTION short -> Main
- Любой экран + UP+DOWN hold 2s -> InfoScreen
- Любой экран + ACTION hold 3s -> Suppress + force Suppress action

## Разделение логики
- Platform-independent: `domain/*`, `game/rules`, `game/mutations`, `ui/ui_fsm`.
- Hardware-specific: `drivers/*`, `input/button_manager`, `storage/state_storage`, `ui/renderer`, `output/output_manager`.

## Mermaid: component
```mermaid
graph LR
  BTN[ButtonManager] --> FSM[UI FSM]
  FSM --> ENG[GameEngine]
  ADXL[ADXL345 Manager] --> ENG
  BH[BH1750 Manager] --> ENG
  ENG --> RULES[Rules + Mutations]
  ENG --> REND[Renderer]
  ENG --> NVS[StateStorage]
  ENG --> OUT[OutputManager]
```

## Mermaid: state diagram
```mermaid
stateDiagram-v2
  [*] --> Main
  Main --> StatusMenu: UP short
  Main --> ActionMenu: DOWN short
  ActionMenu --> Main: ACTION short(confirm)
  StatusMenu --> Main: ACTION short(back)
  InfoScreen --> Main: ACTION short(back)
  Suppress --> Main: ACTION short(back)
  Main --> InfoScreen: UP+DOWN hold 2s
  StatusMenu --> InfoScreen: UP+DOWN hold 2s
  ActionMenu --> InfoScreen: UP+DOWN hold 2s
  Main --> Suppress: ACTION hold 3s
  StatusMenu --> Suppress: ACTION hold 3s
  ActionMenu --> Suppress: ACTION hold 3s
```

## Mermaid: tick processing flow
```mermaid
flowchart TD
  A[Tick timer elapsed] --> B[Read sensors]
  B --> C[applyTick]
  C --> D{shouldDie?}
  D -- yes --> E[resetAfterDeath]
  D -- no --> F[checkMutation]
  E --> G[mark dirty]
  F --> G[mark dirty]
  G --> H{autosave due?}
  H -- yes --> I[saveState]
  H -- no --> J[continue]
```
