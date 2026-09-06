#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "../vec2/vec2.h"
#include "../color/color.h"

typedef enum {
    PROJECTILE_PLAYER = 0,
    PROJECTILE_ENEMY
} ProjectileOwner;

struct GameObject_Projectile {
    struct Vector2 position;
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

struct GameObject_Projectile* projectile_new(float x, float y, float vx, float vy,
                                             char character, Color fg, Color bg,
                                             int damage, float lifetime, ProjectileOwner owner);
void projectile_build(struct GameObject_Projectile *p, float x, float y, float vx, float vy,
                      char character, Color fg, Color bg,
                      int damage, float lifetime, ProjectileOwner owner);
void projectile_free(struct GameObject_Projectile *p);

void projectile_update(struct GameObject_Projectile *p, float dt);

#endif
