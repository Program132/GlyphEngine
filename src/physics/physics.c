#include "physics.h"
#include <stdlib.h>
#include <math.h>

void physics_body_init(struct PhysicsBody *body, float x, float y, float gravity, float bounciness) {
    if (body == NULL) return;
    body->exact_x = x;
    body->exact_y = y;
    body->vx = 0.0f;
    body->vy = 0.0f;
    body->gravity = gravity;
    body->bounciness = bounciness;
    body->friction = 0.85f;
    body->is_grounded = 0;
    body->is_active = 1;
}

void physics_body_apply_force(struct PhysicsBody *body, float fx, float fy) {
    if (body == NULL) return;
    body->vx += fx;
    body->vy += fy;
}

void physics_body_set_velocity(struct PhysicsBody *body, float vx, float vy) {
    if (body == NULL) return;
    body->vx = vx;
    body->vy = vy;
}

static int aabb_overlap(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    return (x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2);
}

void physics_simulate_player(struct GameObject_Player *player, struct Level *level, float move_dir_x, int jump_pressed, float dt) {
    if (player == NULL || level == NULL || dt <= 0.0f) return;

    int pw = (player->texture != NULL) ? player->texture->width : 1;
    int ph = (player->texture != NULL) ? player->texture->height : 1;

    player->vx = move_dir_x * player->speed;

    if (jump_pressed && player->is_grounded) {
        player->vy = -player->jump_power;
        player->is_grounded = 0;
    }

    if (player->use_gravity) {
        player->vy += player->gravity * dt;
        if (player->vy > 50.0f) player->vy = 50.0f;
    }

    float new_x = player->exact_x + player->vx * dt;
    float new_y = player->exact_y;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || !r->is_solid) continue;

        if (aabb_overlap(new_x, new_y, (float)pw, (float)ph, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (player->vx > 0.0f) {
                new_x = (float)r->position.x - (float)pw;
            } else if (player->vx < 0.0f) {
                new_x = (float)(r->position.x + r->width);
            }
            player->vx = 0.0f;
        }
    }

    if (new_x < 0.0f) {
        new_x = 0.0f;
        player->vx = 0.0f;
    }
    if (new_x + (float)pw > (float)level->sizeX) {
        new_x = (float)level->sizeX - (float)pw;
        player->vx = 0.0f;
    }

    player->exact_x = new_x;

    new_y = player->exact_y + player->vy * dt;
    player->is_grounded = 0;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || !r->is_solid) continue;

        if (aabb_overlap(player->exact_x, new_y, (float)pw, (float)ph, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (player->vy > 0.0f) {
                new_y = (float)r->position.y - (float)ph;
                player->vy = 0.0f;
                player->is_grounded = 1;
            } else if (player->vy < 0.0f) {
                new_y = (float)(r->position.y + r->height);
                player->vy = 0.0f;
            }
        }
    }

    if (new_y < 0.0f) {
        new_y = 0.0f;
        player->vy = 0.0f;
    }
    if (new_y + (float)ph >= (float)level->sizeY) {
        new_y = (float)level->sizeY - (float)ph;
        player->vy = 0.0f;
        player->is_grounded = 1;
    }

    player->exact_y = new_y;
    player->position.x = (int)(player->exact_x + 0.5f);
    player->position.y = (int)(player->exact_y + 0.5f);
}

void physics_simulate_rectangle(struct GameObject_Rectangle *rect, struct PhysicsBody *body, struct Level *level, float dt) {
    if (rect == NULL || body == NULL || !body->is_active || level == NULL || dt <= 0.0f) return;

    body->vy += body->gravity * dt;
    if (body->vy > 50.0f) body->vy = 50.0f;

    float w = (float)rect->width;
    float h = (float)rect->height;

    float new_x = body->exact_x + body->vx * dt;
    float new_y = body->exact_y;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || r == rect || !r->is_solid) continue;

        if (aabb_overlap(new_x, new_y, w, h, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (body->vx > 0.0f) {
                new_x = (float)r->position.x - w;
            } else if (body->vx < 0.0f) {
                new_x = (float)(r->position.x + r->width);
            }
            body->vx = -body->vx * body->bounciness;
        }
    }

    if (new_x < 0.0f) {
        new_x = 0.0f;
        body->vx = -body->vx * body->bounciness;
    }
    if (new_x + w > (float)level->sizeX) {
        new_x = (float)level->sizeX - w;
        body->vx = -body->vx * body->bounciness;
    }
    body->exact_x = new_x;

    new_y = body->exact_y + body->vy * dt;
    body->is_grounded = 0;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || r == rect || !r->is_solid) continue;

        if (aabb_overlap(body->exact_x, new_y, w, h, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (body->vy > 0.0f) {
                new_y = (float)r->position.y - h;
                if (fabsf(body->vy) > 3.0f && body->bounciness > 0.05f) {
                    body->vy = -body->vy * body->bounciness;
                } else {
                    body->vy = 0.0f;
                    body->is_grounded = 1;
                }
            } else if (body->vy < 0.0f) {
                new_y = (float)(r->position.y + r->height);
                body->vy = -body->vy * body->bounciness;
            }
        }
    }

    if (new_y < 0.0f) {
        new_y = 0.0f;
        body->vy = -body->vy * body->bounciness;
    }
    if (new_y + h >= (float)level->sizeY) {
        new_y = (float)level->sizeY - h;
        if (fabsf(body->vy) > 3.0f && body->bounciness > 0.05f) {
            body->vy = -body->vy * body->bounciness;
        } else {
            body->vy = 0.0f;
            body->is_grounded = 1;
        }
    }
    body->exact_y = new_y;

    if (body->is_grounded) {
        body->vx *= body->friction;
        if (fabsf(body->vx) < 0.1f) body->vx = 0.0f;
    }

    rect->position.x = (int)(body->exact_x + 0.5f);
    rect->position.y = (int)(body->exact_y + 0.5f);
}

void physics_simulate_ellipse(struct GameObject_Ellipse *ell, struct PhysicsBody *body, struct Level *level, float dt) {
    if (ell == NULL || body == NULL || !body->is_active || level == NULL || dt <= 0.0f) return;

    body->vy += body->gravity * dt;
    if (body->vy > 50.0f) body->vy = 50.0f;

    float w = (float)ell->width;
    float h = (float)ell->height;

    float new_x = body->exact_x + body->vx * dt;
    float new_y = body->exact_y;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || !r->is_solid) continue;

        if (aabb_overlap(new_x, new_y, w, h, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (body->vx > 0.0f) {
                new_x = (float)r->position.x - w;
            } else if (body->vx < 0.0f) {
                new_x = (float)(r->position.x + r->width);
            }
            body->vx = -body->vx * body->bounciness;
        }
    }

    if (new_x < 0.0f) {
        new_x = 0.0f;
        body->vx = -body->vx * body->bounciness;
    }
    if (new_x + w > (float)level->sizeX) {
        new_x = (float)level->sizeX - w;
        body->vx = -body->vx * body->bounciness;
    }
    body->exact_x = new_x;

    new_y = body->exact_y + body->vy * dt;
    body->is_grounded = 0;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || !r->is_solid) continue;

        if (aabb_overlap(body->exact_x, new_y, w, h, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (body->vy > 0.0f) {
                new_y = (float)r->position.y - h;
                if (fabsf(body->vy) > 3.0f && body->bounciness > 0.05f) {
                    body->vy = -body->vy * body->bounciness;
                } else {
                    body->vy = 0.0f;
                    body->is_grounded = 1;
                }
            } else if (body->vy < 0.0f) {
                new_y = (float)(r->position.y + r->height);
                body->vy = -body->vy * body->bounciness;
            }
        }
    }

    if (new_y < 0.0f) {
        new_y = 0.0f;
        body->vy = -body->vy * body->bounciness;
    }
    if (new_y + h >= (float)level->sizeY) {
        new_y = (float)level->sizeY - h;
        if (fabsf(body->vy) > 3.0f && body->bounciness > 0.05f) {
            body->vy = -body->vy * body->bounciness;
        } else {
            body->vy = 0.0f;
            body->is_grounded = 1;
        }
    }
    body->exact_y = new_y;

    if (body->is_grounded) {
        body->vx *= body->friction;
        if (fabsf(body->vx) < 0.1f) body->vx = 0.0f;
    }

    ell->position.x = (int)(body->exact_x + 0.5f);
    ell->position.y = (int)(body->exact_y + 0.5f);
}

void physics_simulate_point(struct GameObject_Point *pt, struct PhysicsBody *body, struct Level *level, float dt) {
    if (pt == NULL || body == NULL || !body->is_active || level == NULL || dt <= 0.0f) return;

    body->vy += body->gravity * dt;
    if (body->vy > 50.0f) body->vy = 50.0f;

    float new_x = body->exact_x + body->vx * dt;
    float new_y = body->exact_y;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || !r->is_solid) continue;

        if (aabb_overlap(new_x, new_y, 1.0f, 1.0f, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (body->vx > 0.0f) {
                new_x = (float)r->position.x - 1.0f;
            } else if (body->vx < 0.0f) {
                new_x = (float)(r->position.x + r->width);
            }
            body->vx = -body->vx * body->bounciness;
        }
    }

    if (new_x < 0.0f) {
        new_x = 0.0f;
        body->vx = -body->vx * body->bounciness;
    }
    if (new_x >= (float)level->sizeX) {
        new_x = (float)level->sizeX - 1.0f;
        body->vx = -body->vx * body->bounciness;
    }
    body->exact_x = new_x;

    new_y = body->exact_y + body->vy * dt;
    body->is_grounded = 0;

    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *r = level->rectangles[i];
        if (r == NULL || !r->is_solid) continue;

        if (aabb_overlap(body->exact_x, new_y, 1.0f, 1.0f, (float)r->position.x, (float)r->position.y, (float)r->width, (float)r->height)) {
            if (body->vy > 0.0f) {
                new_y = (float)r->position.y - 1.0f;
                if (fabsf(body->vy) > 2.0f && body->bounciness > 0.05f) {
                    body->vy = -body->vy * body->bounciness;
                } else {
                    body->vy = 0.0f;
                    body->is_grounded = 1;
                }
            } else if (body->vy < 0.0f) {
                new_y = (float)(r->position.y + r->height);
                body->vy = -body->vy * body->bounciness;
            }
        }
    }

    if (new_y < 0.0f) {
        new_y = 0.0f;
        body->vy = -body->vy * body->bounciness;
    }
    if (new_y >= (float)level->sizeY) {
        new_y = (float)level->sizeY - 1.0f;
        if (fabsf(body->vy) > 2.0f && body->bounciness > 0.05f) {
            body->vy = -body->vy * body->bounciness;
        } else {
            body->vy = 0.0f;
            body->is_grounded = 1;
        }
    }
    body->exact_y = new_y;

    if (body->is_grounded) {
        body->vx *= body->friction;
        if (fabsf(body->vx) < 0.1f) body->vx = 0.0f;
    }

    pt->position.x = (int)(body->exact_x + 0.5f);
    pt->position.y = (int)(body->exact_y + 0.5f);
}
