#ifndef POINT_H
#define POINT_H

#include "../../vec2/vec2.h"
#include "../../color/color.h"

struct GameObject_Point {
    struct Vector2 position;
    char character;
    Color fg;
    Color bg;
};

struct GameObject_Point* point_new(int x, int y, char character);
void point_build(struct GameObject_Point *point, int x, int y, char character);
void point_free(struct GameObject_Point *point);
struct Vector2 point_get_position(struct GameObject_Point *point);
void point_set_position(struct GameObject_Point *point, struct Vector2 newPosition);
char point_get_character(struct GameObject_Point *point);
void point_set_character(struct GameObject_Point *point, char newCharacter);
void point_set_color(struct GameObject_Point *point, Color fg, Color bg);
Color point_get_fg(struct GameObject_Point *point);
Color point_get_bg(struct GameObject_Point *point);

#endif