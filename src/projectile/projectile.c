#include "projectile.h"
#include <stdlib.h>

struct GameObject_Projectile* projectile_new(float x, float y, float vx, float vy,
                                             char character, Color fg, Color bg,
                                             int damage, float lifetime, ProjectileOwner owner) {
    struct GameObject_Projectile *p = (struct GameObject_Projectile*)malloc(sizeof(struct GameObject_Projectile));
    if (p == NULL) return NULL;
    projectile_build(p, x, y, vx, vy, character, fg, bg, damage, lifetime, owner);
    return p;
}

void projectile_build(struct GameObject_Projectile *p, float x, float y, float vx, float vy,
                      char character, Color fg, Color bg,
                      int damage, float lifetime, ProjectileOwner owner) {
    if (p == NULL) return;
    p->exact_x = x;
    p->exact_y = y;
    p->position.x = (int)x;
    p->position.y = (int)y;
    p->vx = vx;
    p->vy = vy;
    p->character = character;
    p->fg = fg;
    p->bg = bg;
    p->damage = damage;
    p->lifetime = lifetime;
    p->is_alive = 1;
    p->owner = owner;
}

void projectile_free(struct GameObject_Projectile *p) {
    if (p != NULL) {
        free(p);
    }
}

void projectile_update(struct GameObject_Projectile *p, float dt) {
    if (p == NULL || !p->is_alive) return;
    p->exact_x += p->vx * dt;
    p->exact_y += p->vy * dt;
    p->position.x = (int)p->exact_x;
    p->position.y = (int)p->exact_y;
    p->lifetime -= dt;
    if (p->lifetime <= 0.0f) {
        p->is_alive = 0;
    }
}
