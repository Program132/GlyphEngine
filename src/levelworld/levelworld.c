#include "levelworld.h"
#include <stdlib.h>

struct LevelWorld* level_world_new(char *name, int world_w, int world_h, int view_w, int view_h, char defaultChar) {
    struct LevelWorld *world = (struct LevelWorld*)malloc(sizeof(struct LevelWorld));
    if (world == NULL) return NULL;
    level_world_build(world, name, world_w, world_h, view_w, view_h, defaultChar);
    return world;
}

void level_world_build(struct LevelWorld *world, char *name, int world_w, int world_h, int view_w, int view_h, char defaultChar) {
    if (world == NULL) return;
    level_build(&world->base, name, world_w, world_h, defaultChar);
    world->world_width = world_w;
    world->world_height = world_h;
    world->viewport_width = view_w;
    world->viewport_height = view_h;
    world->camera_smoothing = 8.0f;
    level_enable_camera(&world->base, view_w, view_h);
}

void level_world_free(struct LevelWorld *world) {
    if (world == NULL) return;

    if (world->base.points != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (world->base.points[i] != NULL) {
                point_free(world->base.points[i]);
            }
        }
        free(world->base.points);
    }

    if (world->base.rectangles != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (world->base.rectangles[i] != NULL) {
                rectangle_free(world->base.rectangles[i]);
            }
        }
        free(world->base.rectangles);
    }

    if (world->base.ellipses != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (world->base.ellipses[i] != NULL) {
                ellipse_free(world->base.ellipses[i]);
            }
        }
        free(world->base.ellipses);
    }

    if (world->base.players != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (world->base.players[i] != NULL) {
                player_free(world->base.players[i]);
            }
        }
        free(world->base.players);
    }

    if (world->base.texts != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (world->base.texts[i] != NULL) {
                text_free(world->base.texts[i]);
            }
        }
        free(world->base.texts);
    }

    for (int i = 0; i < MAX_LEVEL_PROJECTILES; i++) {
        if (world->base.projectiles[i] != NULL) {
            projectile_free(world->base.projectiles[i]);
            world->base.projectiles[i] = NULL;
        }
    }

    if (world->base.particle_system != NULL) {
        particle_system_free(world->base.particle_system);
    }

    free(world);
}

void level_world_set_camera(struct LevelWorld *world, float cam_x, float cam_y) {
    if (world == NULL) return;
    world->base.cam_x = cam_x;
    world->base.cam_y = cam_y;
    level_world_clamp_camera(world);
}

void level_world_clamp_camera(struct LevelWorld *world) {
    if (world == NULL) return;
    float max_x = (float)(world->world_width - world->viewport_width);
    float max_y = (float)(world->world_height - world->viewport_height);
    if (max_x < 0.0f) max_x = 0.0f;
    if (max_y < 0.0f) max_y = 0.0f;

    if (world->base.cam_x < 0.0f) world->base.cam_x = 0.0f;
    if (world->base.cam_x > max_x) world->base.cam_x = max_x;
    if (world->base.cam_y < 0.0f) world->base.cam_y = 0.0f;
    if (world->base.cam_y > max_y) world->base.cam_y = max_y;
}

void level_world_follow_player(struct LevelWorld *world, struct GameObject_Player *player, float smoothing, float dt) {
    if (world == NULL || player == NULL) return;

    int pw = (player->texture != NULL) ? player->texture->width : 1;
    int ph = (player->texture != NULL) ? player->texture->height : 1;

    float target_cam_x = player->exact_x + (float)pw * 0.5f - (float)world->viewport_width * 0.5f;
    float target_cam_y = player->exact_y + (float)ph * 0.5f - (float)world->viewport_height * 0.5f;

    if (smoothing <= 0.0f || dt <= 0.0f) {
        world->base.cam_x = target_cam_x;
        world->base.cam_y = target_cam_y;
    } else {
        float factor = smoothing * dt;
        if (factor > 1.0f) factor = 1.0f;
        world->base.cam_x += (target_cam_x - world->base.cam_x) * factor;
        world->base.cam_y += (target_cam_y - world->base.cam_y) * factor;
    }

    level_world_clamp_camera(world);
}

struct Level* level_world_as_level(struct LevelWorld *world) {
    if (world == NULL) return NULL;
    return &world->base;
}
