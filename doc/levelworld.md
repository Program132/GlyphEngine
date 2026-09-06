# LevelWorld & Camera Documentation

The **`LevelWorld`** module extends `Level` to support maps larger than the console terminal. It introduces viewport-based rendering and a dynamic **Camera** that smoothly follows the player or pans across large landscapes.

Because `struct LevelWorld` inherits `struct Level` directly (`Level base` is the first member), any `LevelWorld` instance is **100% compatible** with all existing engine systems (`engine_set_level`, `collision_clamp_player`, `level_add_*`, etc.).

---

## Headers

```c
#include "src/levelworld/levelworld.h"
#include "src/level/level.h"
#include "src/engine/engine.h"
```

---

## Data Structure

```c
struct LevelWorld {
    struct Level base;
    int world_width;
    int world_height;
    int viewport_width;
    int viewport_height;
    float camera_smoothing;
};
```

---

## API Reference

### Creation & Destruction
- `struct LevelWorld* level_world_new(char *name, int world_w, int world_h, int view_w, int view_h, char defaultChar);`
  Allocates a large world level with the specified world map dimensions (`world_w`, `world_h`) and viewport window dimensions (`view_w`, `view_h`).
- `void level_world_build(struct LevelWorld *world, char *name, int world_w, int world_h, int view_w, int view_h, char defaultChar);`
  Initializes an existing `LevelWorld` instance.
- `void level_world_free(struct LevelWorld *world);`
  Frees all world arrays, entities, projectiles, and particle systems.

### Camera Controls & Smooth Follow
- `void level_world_set_camera(struct LevelWorld *world, float cam_x, float cam_y);`
  Directly moves the top-left camera viewport coordinate (automatically clamped to world bounds).
- `void level_world_clamp_camera(struct LevelWorld *world);`
  Ensures the camera never looks outside the world map boundary: `[0, world_width - viewport_width]` and `[0, world_height - viewport_height]`.
- `void level_world_follow_player(struct LevelWorld *world, struct GameObject_Player *player, float smoothing, float dt);`
  Smoothly interpolates the camera towards the player's center position using delta-time based smoothing. If `smoothing <= 0`, it snaps instantly.

### Conversion / Base Access
- `struct Level* level_world_as_level(struct LevelWorld *world);`
  Returns a pointer to the underlying `Level` to pass to `engine_set_level`, `level_add_*`, etc. Alternatively, `&world->base` or casting `(struct Level*)world` can be used.

---

## Usage Example

```c
// Create a 140x50 world with a 60x18 viewport on screen
struct LevelWorld *world = level_world_new("Dungeon", 140, 50, 60, 18, '.');
struct Level *lvl = level_world_as_level(world);

// Add objects anywhere in the 140x50 world
level_add_player(lvl, player);
level_add_rectangle(lvl, fortress);

// In the update loop:
void update(struct Engine *engine, float dt) {
    player_move(player, dx, dy, dt);
    collision_clamp_player(lvl, player);
    
    // Camera smoothly tracks player
    level_world_follow_player(world, player, 6.0f, dt);
}
```
