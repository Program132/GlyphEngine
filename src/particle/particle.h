#ifndef PARTICLE_H
#define PARTICLE_H

#include "../vec2/vec2.h"
#include "../color/color.h"

#define MAX_PARTICLES 256

struct Particle {
    float exact_x;
    float exact_y;
    int x;
    int y;
    float vx;
    float vy;
    char character;
    Color fg;
    Color bg;
    float lifetime;
    int is_alive;
};

struct ParticleSystem {
    struct Particle particles[MAX_PARTICLES];
    int active_count;
};

struct ParticleSystem* particle_system_new(void);
void particle_system_build(struct ParticleSystem *ps);
void particle_system_free(struct ParticleSystem *ps);

void particle_system_update(struct ParticleSystem *ps, float dt);
void particle_system_emit(struct ParticleSystem *ps, float x, float y, float vx, float vy,
                          char character, Color fg, Color bg, float lifetime);

void particle_system_burst_explosion(struct ParticleSystem *ps, float x, float y, int count, Color fg);
void particle_system_burst_sparkle(struct ParticleSystem *ps, float x, float y, int count, Color fg);
void particle_system_clear(struct ParticleSystem *ps);

#endif
