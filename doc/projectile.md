# Projectile System Documentation

The **Projectile System** provides high-speed dynamic entities in GlyphEngine. Projectiles support floating-point velocities for smooth sub-pixel trajectories, customizable ASCII visuals, ANSI foreground colors, lifetime limits, and ownership tags (`PROJECTILE_PLAYER` vs `PROJECTILE_ENEMY`).

---

## Headers

```c
#include "src/projectile/projectile.h"
#include "src/level/level.h"
#include "src/collision/collision.h"
```

---

## Data Structures

### `ProjectileOwner`
An enumeration defining who fired the projectile to filter collision checks:
- `PROJECTILE_NONE`
- `PROJECTILE_PLAYER`
- `PROJECTILE_ENEMY`

### `GameObject_Projectile`
```c
struct GameObject_Projectile {
    float exact_x;
    float exact_y;
    float vx;
    float vy;
    char character;
    Color fg;
    Color bg;
    int damage;
    float lifetime;
    int is_alive;
    ProjectileOwner owner;
};
```

---

## API Reference

### Creation and Destruction
- `struct GameObject_Projectile *projectile_new(float x, float y, float vx, float vy, char character, Color fg, Color bg, int damage, float lifetime, ProjectileOwner owner);`
  Allocates and initializes a new projectile on the heap.
- `void projectile_build(struct GameObject_Projectile *proj, float x, float y, float vx, float vy, char character, Color fg, Color bg, int damage, float lifetime, ProjectileOwner owner);`
  Initializes or resets an existing projectile structure.
- `void projectile_free(struct GameObject_Projectile *proj);`
  Frees projectile memory.
- `void projectile_update(struct GameObject_Projectile *proj, float dt);`
  Updates position (`exact_x += vx * dt`, `exact_y += vy * dt`) and decreases remaining lifetime. Sets `is_alive = 0` when lifetime expires.

---

## Spawning via Level

The recommended way to use projectiles is directly through the `Level` interface:

```c
void level_spawn_projectile(struct Level *level,
                            float x, float y,
                            float vx, float vy,
                            char character,
                            Color fg,
                            int damage,
                            float lifetime,
                            ProjectileOwner owner);
```

When called, `level_spawn_projectile` searches an internal pool of up to `MAX_LEVEL_PROJECTILES` (128) and reuses dead projectile slots before allocating new ones.

---

## Collision Detection

Check projectile hits against other game objects:

```c
int collision_check_projectile_rect(struct GameObject_Projectile *proj, struct GameObject_Rectangle *rect);
int collision_check_projectile_ellipse(struct GameObject_Projectile *proj, struct GameObject_Ellipse *ellipse);
int collision_check_projectile_player(struct GameObject_Projectile *proj, struct GameObject_Player *player);
```

### Example Usage
```c
if (proj->is_alive && proj->owner == PROJECTILE_PLAYER) {
    if (collision_check_projectile_rect(proj, enemy_rect)) {
        proj->is_alive = 0;
        enemy_hp -= proj->damage;
        level_spawn_particles_explosion(level, proj->exact_x, proj->exact_y, 12, COLOR_BRIGHT_RED);
    }
}
```
