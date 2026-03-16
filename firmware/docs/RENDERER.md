# RENDERER

## Renderer 2.0
Добавлены системные слои:
- `ui/theme.h` — палитра,
- `ui/layout.h` — layout constants,
- `ui/icons.h` — form icons (placeholder strategy),
- `ui/animation_state.h` — pulse/blink.

## Что отображается
- form icon,
- stat bars,
- warnings (blink),
- transient overlay,
- cooldown status для выбранного action.

## Ограничения
- Иконки пока процедурные placeholder primitives.
- Для production assets можно добавить sprite atlas без ломки интерфейса.
