#ifndef PHYSICS_H
#define PHYSICS_H

#include "../level/level.h"
#include "../gameobject/player/player.h"
#include "../gameobject/rectangle/rectangle.h"
#include "../gameobject/ellipse/ellipse.h"
#include "../gameobject/point/point.h"

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

void physics_body_init(struct PhysicsBody *body, float x, float y, float gravity, float bounciness);
void physics_body_apply_force(struct PhysicsBody *body, float fx, float fy);
void physics_body_set_velocity(struct PhysicsBody *body, float vx, float vy);

void physics_simulate_player(struct GameObject_Player *player, struct Level *level, float move_dir_x, int jump_pressed, float dt);
void physics_simulate_rectangle(struct GameObject_Rectangle *rect, struct PhysicsBody *body, struct Level *level, float dt);
void physics_simulate_ellipse(struct GameObject_Ellipse *ell, struct PhysicsBody *body, struct Level *level, float dt);
void physics_simulate_point(struct GameObject_Point *pt, struct PhysicsBody *body, struct Level *level, float dt);

#endif
