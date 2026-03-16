# MIGRATIONS

## Версии схемы
- v1: базовые статы + form/age/deaths
- v2: добавлены `sleepTicksRemaining`, `suppressTicksRemaining`
- v3: добавлены cooldown-поля действий

Текущая версия: `config::app::kStateVersion`.

## Pipeline
`StateStorage::loadState()`:
1. Читает envelope (`saved`, `ver`).
2. Загружает поля по версии.
3. Запускает `migrateToLatest()`.
4. Нормализует и валидирует state.
5. При ошибке использует default state.

## Recovery telemetry
Фиксируются события:
- migration applied,
- unsupported version,
- fallback to defaults,
- corrupted state recovered.

## Как добавить v4
1. Добавить migrate step в `src/storage/migrations.cpp`.
2. Поднять `kStateVersion`.
3. Добавить чтение/запись новых полей в storage.
4. Добавить тест migration path.
