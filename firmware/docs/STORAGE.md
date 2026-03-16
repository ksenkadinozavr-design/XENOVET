# STORAGE

## Storage API
- `begin()`
- `hasSavedState()`
- `loadState()` -> `StorageLoadResult`
- `saveState(const CreatureState&)`
- `resetState()`

## Формат
Используется NVS namespace `xenovent`.
Хранится envelope:
- `saved` flag,
- `ver` (state format version),
- поля `CreatureState`.

## Version guard
Если версия в NVS не совпадает с `config::app::kStateVersion`, загрузка откатывается к default state и помечается `versionMismatch=true`.

## Когда сохраняем
- В tick/action pipeline выставляется `dirty`.
- Engine выполняет autosave по интервалу `kAutosaveIntervalMs`.

## Повреждённые/отсутствующие данные
- Нет сохранения -> default state.
- Несовместимая версия -> default state + warning log.


## Cooldown fields (v3)
Persisted as per-action counters (`feed/suppress/ritual/meditate/sleep`) and restored on boot.
Old schemas (v1/v2) migrate to v3 with default cooldown values.
