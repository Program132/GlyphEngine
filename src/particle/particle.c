#include "particle.h"
#include <stdlib.h>
#include <math.h>

struct ParticleSystem* particle_system_new(void) {
    struct ParticleSystem *ps = (struct ParticleSystem*)malloc(sizeof(struct ParticleSystem));
    if (ps == NULL) return NULL;
    particle_system_build(ps);
    return ps;
}

void particle_system_build(struct ParticleSystem *ps) {
    if (ps == NULL) return;
    ps->active_count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        ps->particles[i].is_alive = 0;
        ps->particles[i].lifetime = 0.0f;
    }
}

void particle_system_free(struct ParticleSystem *ps) {
    if (ps != NULL) {
        free(ps);
    }
}

void particle_system_update(struct ParticleSystem *ps, float dt) {
    if (ps == NULL) return;

    ps->active_count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!ps->particles[i].is_alive) continue;

        ps->particles[i].exact_x += ps->particles[i].vx * dt;
        ps->particles[i].exact_y += ps->particles[i].vy * dt;
        ps->particles[i].x = (int)ps->particles[i].exact_x;
        ps->particles[i].y = (int)ps->particles[i].exact_y;
        ps->particles[i].lifetime -= dt;

        if (ps->particles[i].lifetime <= 0.0f) {
            ps->particles[i].is_alive = 0;
        } else {
            ps->active_count++;
        }
    }
}

void particle_system_emit(struct ParticleSystem *ps, float x, float y, float vx, float vy,
                          char character, Color fg, Color bg, float lifetime) {
    if (ps == NULL) return;

    int slot = -1;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!ps->particles[i].is_alive) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        slot = rand() % MAX_PARTICLES;
    }

    struct Particle *p = &ps->particles[slot];
    p->exact_x = x;
    p->exact_y = y;
    p->x = (int)x;
    p->y = (int)y;
    p->vx = vx;
    p->vy = vy;
    p->character = character;
    p->fg = fg;
    p->bg = bg;
    p->lifetime = lifetime;
    p->is_alive = 1;
}

void particle_system_burst_explosion(struct ParticleSystem *ps, float x, float y, int count, Color fg) {
    if (ps == NULL || count <= 0) return;

    static const char explosion_chars[] = {'*', '+', '.', '#', 'o', 'x'};
    int char_count = sizeof(explosion_chars);

    for (int i = 0; i < count; i++) {
        float angle = (float)(rand() % 360) * (3.14159265f / 180.0f);
        float speed = 8.0f + (float)(rand() % 16);
        float vx = cosf(angle) * speed;
        float vy = sinf(angle) * (speed * 0.55f);
        char ch = explosion_chars[rand() % char_count];
        float life = 0.3f + (float)(rand() % 40) / 100.0f;
        Color p_fg = (rand() % 3 == 0) ? COLOR_BRIGHT_YELLOW : ((rand() % 2 == 0) ? fg : COLOR_BRIGHT_WHITE);
        particle_system_emit(ps, x, y, vx, vy, ch, p_fg, COLOR_DEFAULT, life);
    }
}

void particle_system_burst_sparkle(struct ParticleSystem *ps, float x, float y, int count, Color fg) {
    if (ps == NULL || count <= 0) return;

    static const char sparkle_chars[] = {'*', '.', '+'};
    int char_count = sizeof(sparkle_chars);

    for (int i = 0; i < count; i++) {
        float vx = -6.0f + (float)(rand() % 13);
        float vy = -3.0f - (float)(rand() % 8);
        char ch = sparkle_chars[rand() % char_count];
        float life = 0.4f + (float)(rand() % 40) / 100.0f;
        Color p_fg = (rand() % 2 == 0) ? fg : COLOR_BRIGHT_WHITE;
        particle_system_emit(ps, x, y, vx, vy, ch, p_fg, COLOR_DEFAULT, life);
    }
}

void particle_system_clear(struct ParticleSystem *ps) {
    if (ps == NULL) return;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        ps->particles[i].is_alive = 0;
        ps->particles[i].lifetime = 0.0f;
    }
    ps->active_count = 0;
}
