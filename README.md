# GlyphEngine

Minimalist 2D text-based game engine for the terminal (C99).

## Features

- Double-buffered console rendering (flicker-free)
- Delta time calculation and target framerate cap
- Real-time keyboard input polling (non-blocking)
- 2D ASCII Texturing system with transparency support
- Dedicated `GameObject_Player` with sub-pixel delta movement and health
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
#include "src/gameobject/player/player.h"
#include "src/texture/texture.h"

struct GameObject_Player* player;

void update(struct Engine* engine, float dt) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    float dx = 0.0f;
    float dy = 0.0f;

    if (input_is_key_down(KEY_Z) || input_is_key_down(KEY_UP))    dy -= 1.0f;
    if (input_is_key_down(KEY_S) || input_is_key_down(KEY_DOWN))  dy += 1.0f;
    if (input_is_key_down(KEY_Q) || input_is_key_down(KEY_LEFT))  dx -= 1.0f;
    if (input_is_key_down(KEY_D) || input_is_key_down(KEY_RIGHT)) dx += 1.0f;

    player_move(player, dx, dy, dt);
}

int main() {
    struct Level* level = level_new("My Game", 60, 20, '.');
    struct Engine* engine = engine_new(level, 60, 20);

    const char* hero_sprite = 
        " o \n"
        "/|\\\n"
        "/ \\";
    struct Texture* hero_tex = texture_new(3, 3, hero_sprite, ' ');

    player = player_new_textured(10, 8, hero_tex, 15.0f);
    level_add_player(level, player);

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
- **Texturing & Graphics**
  - [Texture](doc/texture.md) — Multi-character ASCII art patterns and transparency.
- **Game Objects**
  - [Player](doc/GameObject_Player.md) — Dedicated player entity with sub-pixel movement and health.
  - [Point](doc/GameObject_Points.md) — Single character point.
  - [Rectangle](doc/GameObject_Rectangles.md) — Solid or textured rectangle.
  - [Square](doc/GameObject_Squares.md) — Square helper for rectangle.
  - [Ellipse](doc/GameObject_Ellipses.md) — Filled or outlined ellipse.
  - [Circle](doc/GameObject_Circles.md) — Circle helper for ellipse.
- **Math**
  - [Vector2](doc/vec2.md) — 2D coordinate structure.

## Examples

Example programs are available in the `examples/` directory:
- `examples/01_player_movement.c`: Basic keyboard movement with an obstacle.
- `examples/02_bouncing_shapes.c`: Bouncing geometric shapes.
- `examples/03_textured_player.c`: Textured player sprite with smooth delta-time movement.
- `examples/04_textured_world.c`: Textured platforms, brick pillars, and moving character.
- `examples/05_dungeon_crawler.c`: Dungeon crawler with textured walls, player health, and items.