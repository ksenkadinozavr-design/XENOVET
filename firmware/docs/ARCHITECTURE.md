# ARCHITECTURE

## Layered model
1. **Config layer** (`include/config/*`) — пины, баланс, UI тайминги, app constants.
2. **Domain layer** (`include/domain`, `src/domain`) — чистая логика состояний/действий/мутаций.
3. **Game layer** (`include/game`, `src/game`) — tick pipeline, orchestration и интеграция сервисов.
4. **UI layer** (`include/ui`, `src/ui`) — UI model/FSM/render model.
5. **Platform/hardware layer** (`drivers`, `input`, `output`) — адаптеры железа.
6. **Storage layer** (`storage`) — NVS persistence + version guard.
7. **Diagnostics layer** (`utils/logger`) — уровневые логи.
8. **Telemetry layer** (`telemetry/*`) — event bus, recorder, replay hooks.

## Boundaries
- Domain не зависит от Arduino/ST7789/Preferences.
- GameEngine оркестрирует сервисы, но не знает деталей draw API.
- Input interpreter отделён от GPIO polling.
- Storage изолирован за `StateStorage` API.

## Tick flow
```mermaid
flowchart TD
  A[GameEngine tick timer] --> B[Build TickContext]
  B --> C[domain::applyTick]
  C --> D[deriveUiFlags]
  D --> E[shouldDie]
  E -- dead --> F[resetAfterDeath]
  E -- alive --> G[checkMutation]
  F --> H[TickResult]
  G --> H[TickResult]
  H --> I[mark dirty + feedback]
```

## Startup sequence
```mermaid
sequenceDiagram
  participant Main
  participant Engine
  participant Storage
  participant HW as HW services
  participant UI

  Main->>Engine: begin()
  Engine->>Storage: begin(), hasSavedState(), loadState()
  Engine->>HW: buttons/sensors/output begin()
  Engine->>UI: renderer begin()
  Engine-->>Main: ready
```

## UI state diagram
```mermaid
stateDiagram-v2
  [*] --> Main
  Main --> StatusMenu: UP short
  Main --> ActionMenu: DOWN short
  ActionMenu --> Main: ACTION short(confirm)
  StatusMenu --> Main: ACTION short(back)
  InfoScreen --> Main: ACTION short(back)
  Suppress --> Main: ACTION short(back)
  Main --> InfoScreen: UP+DOWN hold
  StatusMenu --> InfoScreen: UP+DOWN hold
  ActionMenu --> InfoScreen: UP+DOWN hold
  Main --> Suppress: ACTION long
  StatusMenu --> Suppress: ACTION long
  ActionMenu --> Suppress: ACTION long
```


## Storage schema evolution
- Storage uses versioned envelopes and migration pipeline (v1->v2->v3).
- Recovery path emits telemetry and falls back to safe defaults when migration is unsupported.
