#ifndef LEVELWORLD_H
#define LEVELWORLD_H

#include "../level/level.h"
#include "../gameobject/player/player.h"

struct LevelWorld {
    struct Level base;
    int world_width;
    int world_height;
    int viewport_width;
    int viewport_height;
    float camera_smoothing;
};

struct LevelWorld* level_world_new(char *name, int world_w, int world_h, int view_w, int view_h, char defaultChar);
void level_world_build(struct LevelWorld *world, char *name, int world_w, int world_h, int view_w, int view_h, char defaultChar);
void level_world_free(struct LevelWorld *world);

void level_world_set_camera(struct LevelWorld *world, float cam_x, float cam_y);
void level_world_follow_player(struct LevelWorld *world, struct GameObject_Player *player, float smoothing, float dt);
void level_world_clamp_camera(struct LevelWorld *world);

struct Level* level_world_as_level(struct LevelWorld *world);

#endif
