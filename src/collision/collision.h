#ifndef COLLISION_H
#define COLLISION_H

#include "../vec2/vec2.h"
#include "../gameobject/rectangle/rectangle.h"
#include "../gameobject/ellipse/ellipse.h"
#include "../gameobject/player/player.h"

struct Level;

int collision_check_point_rect(struct Vector2 point, struct GameObject_Rectangle *rect);
int collision_check_rect_rect(struct GameObject_Rectangle *r1, struct GameObject_Rectangle *r2);
int collision_check_point_ellipse(struct Vector2 point, struct GameObject_Ellipse *ellipse);
int collision_check_rect_ellipse(struct GameObject_Rectangle *rect, struct GameObject_Ellipse *ellipse);
int collision_check_ellipse_ellipse(struct GameObject_Ellipse *e1, struct GameObject_Ellipse *e2);

#include "../gameobject/point/point.h"

int collision_check_player_rect(struct GameObject_Player *player, struct GameObject_Rectangle *rect);
int collision_check_player_ellipse(struct GameObject_Player *player, struct GameObject_Ellipse *ellipse);
int collision_check_player_point(struct GameObject_Player *player, struct GameObject_Point *point);
int collision_check_player_player(struct GameObject_Player *p1, struct GameObject_Player *p2);

int collision_is_out_of_bounds(struct Level *level, int x, int y);
void collision_clamp_player(struct Level *level, struct GameObject_Player *player);

#endif
