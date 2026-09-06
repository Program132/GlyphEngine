# Physics System Documentation

The **Physics System** provides 2D simulation for platformers and physics-driven arcade mechanics in GlyphEngine. It supports universal gravity on standard game objects (`GameObject_Rectangle`, `GameObject_Ellipse`, `GameObject_Point`, `GameObject_Player`), ground detection, jumping mechanics, solid collision resolution against platforms, and bouncing.

---

## Headers

```c
#include "src/physics/physics.h"
#include "src/gameobject/player/player.h"
#include "src/gameobject/rectangle/rectangle.h"
```

---

## Physics Body Component (`PhysicsBody`)

```c
struct PhysicsBody {
    float exact_x;
    float exact_y;
    float vx;
    float vy;
    float gravity;
    float bounciness;
    float friction;
    int is_grounded;
    int is_active;
};
```

### Methods
- `void physics_body_init(struct PhysicsBody *body, float x, float y, float gravity, float bounciness);`
  Initializes physics state with custom gravity and bounciness restitution (0.0 = stops instantly, 1.0 = full bounce).
- `void physics_body_apply_force(struct PhysicsBody *body, float fx, float fy);`
  Adds impulse/velocity to the body.
- `void physics_body_set_velocity(struct PhysicsBody *body, float vx, float vy);`
  Sets explicit horizontal and vertical speeds.

---

## Player Physics & Jump Power

`GameObject_Player` includes built-in physics properties for smooth platformer mechanics:

- `player_enable_gravity(player, gravity)` / `player_disable_gravity(player)`:
  Enables vertical gravitational acceleration. Disabled by default to keep top-down games unaffected.
- `player_set_jump_power(player, jump_power)` / `player_get_jump_power(player)`:
  Configures the instantaneous upward impulse applied upon jumping.
- `player_set_gravity(player, gravity)` / `player_get_gravity(player)`:
  Sets the downward acceleration in cells/s².
- `player_jump(player)`:
  Triggers a jump if `player_is_grounded(player)` is true.
- `player_is_grounded(player)`:
  Returns `1` if the player is resting on a solid platform or level bottom, `0` if in the air.

---

## Solid Geometry (`is_solid`)

`GameObject_Rectangle` supports collision solid states:
- `void rectangle_set_solid(struct GameObject_Rectangle *rectangle, int is_solid);`
- `int rectangle_is_solid(struct GameObject_Rectangle *rectangle);`

When `is_solid == 1` (default), falling bodies and the player will land on the rectangle's top edge, bump into ceilings, and be blocked by its vertical sides. Non-solid rectangles (`is_solid == 0`) act as background or visual-only elements.

---

## Simulation Solvers

- `void physics_simulate_player(struct GameObject_Player *player, struct Level *level, float move_dir_x, int jump_pressed, float dt);`
  Performs complete discrete axis collision resolution (horizontal then vertical) for the player against all solid rectangles and screen boundaries.
- `void physics_simulate_rectangle(struct GameObject_Rectangle *rect, struct PhysicsBody *body, struct Level *level, float dt);`
  Simulates gravity, solid platform landings, and bounce for dynamic boxes.
- `void physics_simulate_ellipse(struct GameObject_Ellipse *ell, struct PhysicsBody *body, struct Level *level, float dt);`
  Simulates falling and bouncing spheres/balls.
- `void physics_simulate_point(struct GameObject_Point *pt, struct PhysicsBody *body, struct Level *level, float dt);`
  Simulates falling points/items.

---

## Usage Example

```c
// Setup player with gravity and jump power
player_enable_gravity(player, 40.0f);
player_set_jump_power(player, 19.0f);

// Setup a solid ground platform
struct GameObject_Rectangle *ground = rectangle_new(0, 16, 60, 2, '=');
rectangle_set_solid(ground, 1);
level_add_rectangle(level, ground);

// In the engine update callback:
void update(struct Engine *engine, float dt) {
    float move_x = 0.0f;
    if (input_is_key_down(KEY_A)) move_x -= 1.0f;
    if (input_is_key_down(KEY_D)) move_x += 1.0f;
    int jump = input_is_key_down(KEY_SPACE);

    physics_simulate_player(player, level, move_x, jump, dt);
}
```
