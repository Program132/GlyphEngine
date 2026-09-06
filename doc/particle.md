# Particle System Documentation

The **Particle System** in GlyphEngine is a pool-based particle engine designed for terminal environments. It supports ANSI color transitions (dimming or fading based on life ratio), velocity, character glyphs, lifetime management, and preset bursts for explosions and sparkles.

---

## Headers

```c
#include "src/particle/particle.h"
#include "src/level/level.h"
```

---

## Data Structures

### `Particle`
```c
struct Particle {
    float x;
    float y;
    float vx;
    float vy;
    char character;
    Color fg;
    float lifetime;
    float max_lifetime;
    int is_alive;
};
```

### `ParticleSystem`
```c
struct ParticleSystem {
    struct Particle pool[PARTICLE_POOL_SIZE]; /* 256 particles */
    int active_count;
};
```

---

## API Reference

### Creation & Management
- `struct ParticleSystem *particle_system_new(void);`
  Allocates and zero-initializes a new particle system.
- `void particle_system_free(struct ParticleSystem *ps);`
  Frees the particle system.
- `void particle_system_clear(struct ParticleSystem *ps);`
  Instantly deactivates all active particles in the pool.
- `void particle_system_update(struct ParticleSystem *ps, float dt);`
  Steps particle simulation forward: updates positions, ages lifetimes, and deactivates expired particles.

### Emitting Particles
- `void particle_system_emit(struct ParticleSystem *ps, float x, float y, float vx, float vy, char character, Color fg, float lifetime);`
  Emits a single custom particle into the pool.
- `void particle_system_burst_explosion(struct ParticleSystem *ps, float x, float y, int count, Color fg);`
  Emits a burst of particles radially outward with varying velocities and glyphs (`*`, `+`, `o`, `.`).
- `void particle_system_burst_sparkle(struct ParticleSystem *ps, float x, float y, int count, Color fg);`
  Emits a delicate sparkle effect (`.`, `*`) around a coordinate.

---

## Spawning via Level

`Level` integrates a built-in `ParticleSystem`. You can spawn bursts directly:

```c
void level_spawn_particles_explosion(struct Level *level, float x, float y, int count, Color fg);
void level_spawn_particles_sparkle(struct Level *level, float x, float y, int count, Color fg);
```

During `level_update(level, dt)`, active particles are updated.
During `level_display(level)`, active particles are drawn directly to the level screen buffer. As particles age, bright colors automatically fade to standard or darker tones for realistic decay in ANSI terminals.
