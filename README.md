# GlyphEngine

Minimalist 2D text-based game engine for the terminal (C99).

## Features

- Double-buffered console rendering (flicker-free)
- ANSI 16-color palette (foreground & background) with state-change optimization
- 2D Collision detection module (AABB, circle/ellipse, player, clamp, boundaries)
- Delta time calculation and target framerate cap
- Real-time keyboard input polling (non-blocking)
- 2D ASCII Texturing system with transparency support
- In-game Text & HUD labels (`GameObject_Text`) with custom positioning and colors
- Off-canvas HUD system (`HUD_TOP`, `HUD_BOTTOM`) with full-width separators outside the game grid
- Dedicated `LevelUI` scene system with interactive buttons (`UIButton`), styled panels (`UIPanel`), and keyboard navigation
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
#include "src/color/color.h"
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
    level_set_default_color(level, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);

    struct Engine* engine = engine_new(level, 60, 20);

    const char* hero_sprite = 
        " o \n"
        "/|\\\n"
        "/ \\";
    struct Texture* hero_tex = texture_new(3, 3, hero_sprite, ' ');

    player = player_new_textured(10, 8, hero_tex, 15.0f);
    player_set_color(player, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
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
  - [Level](doc/level.md) — Object container, grid, rendering, and off-canvas HUD.
  - [LevelUI](doc/levelui.md) — GUI scenes, buttons, styled panels, and keyboard navigation.
- **Physics & Styling**
  - [Collision](doc/collision.md) — 2D intersection routines, boundary checks, and clamping.
  - [Color](doc/color.md) — ANSI 16-color enumeration and styling helpers.
- **Inputs & Utilities**
  - [Inputs](doc/inputs.md) — Real-time key state polling and keycodes.
  - [Utils](doc/utils.md) — Screen clearing and cursor control.
- **Texturing & Graphics**
  - [Texture](doc/texture.md) — Multi-character ASCII art patterns and transparency.
- **Game Objects & UI Components**
  - [Player](doc/GameObject_Player.md) — Dedicated player entity with sub-pixel movement, health, and colors.
  - [Text](doc/GameObject_Text.md) — Screen text and HUD labels with custom position and ANSI colors.
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
- `examples/06_textured_shapes.c`: Textured circle, ellipse, and square shapes.
- `examples/07_collisions_and_colors.c`: Colored level, colored obstacles, and real-time collision detection.
- `examples/08_level_ui_menu.c`: Interactive GUI menu with buttons, panels, keyboard navigation, and level transition.