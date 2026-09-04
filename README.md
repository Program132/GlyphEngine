# GlyphEngine

Minimalist 2D text-based game engine for the terminal (C99).

## Features

- Double-buffered console rendering (flicker-free)
- Delta time calculation and target framerate cap
- Real-time keyboard input polling (non-blocking)
- 2D geometric shapes (Point, Rectangle, Square, Ellipse, Circle)
- Cascading memory management (`engine_free`)

## Building

Requires `CMake` and a C compiler (`gcc`, `clang`, or `MSVC`).

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

To run:

```bash
# Windows
.\Debug\GlyphEngine.exe

# Linux / macOS
./GlyphEngine
```

## Quickstart

```c
#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/gameobject/square/square.h"

struct GameObject_Rectangle* player;

void update(struct Engine* engine, float dt) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    if (input_is_key_down(KEY_Z) || input_is_key_down(KEY_UP))    player->position.y -= 1;
    if (input_is_key_down(KEY_S) || input_is_key_down(KEY_DOWN))  player->position.y += 1;
    if (input_is_key_down(KEY_Q) || input_is_key_down(KEY_LEFT))  player->position.x -= 1;
    if (input_is_key_down(KEY_D) || input_is_key_down(KEY_RIGHT)) player->position.x += 1;
}

int main() {
    struct Level* level = level_new("My Game", 60, 20, '.');
    struct Engine* engine = engine_new(level, 60, 20);

    player = square_new(5, 5, 3, '#');
    level_add_rectangle(level, player);

    engine_set_update_callback(engine, update);
    engine_run(engine, 30);

    engine_free(engine);

    return 0;
}
```

## Documentation

Detailed documentation for each module is located in the [doc/](doc/) directory:

- **Engine & Scenes**
  - [Engine](doc/engine.md) — Main loop, framerate, and callbacks.
  - [Level](doc/level.md) — Object container, grid, and rendering.
- **Inputs & Utilities**
  - [Inputs](doc/inputs.md) — Real-time key state polling and keycodes.
  - [Utils](doc/utils.md) — Screen clearing and cursor control.
- **Math & Objects**
  - [Vector2](doc/vec2.md) — 2D coordinate structure.
  - [Point](doc/GameObject_Points.md) — Single character point.
  - [Rectangle](doc/GameObject_Rectangles.md) — Filled or outlined rectangle.
  - [Square](doc/GameObject_Squares.md) — Square helper for rectangle.
  - [Ellipse](doc/GameObject_Ellipses.md) — Filled or outlined ellipse.
  - [Circle](doc/GameObject_Circles.md) — Circle helper for ellipse.

## Examples

Example programs are available in the `examples/` directory:
- `examples/01_player_movement.c`: Keyboard player movement with obstacle.
- `examples/02_bouncing_shapes.c`: Bouncing geometric shapes.