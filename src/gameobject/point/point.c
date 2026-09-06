#include "point.h"
#include <stdlib.h>

struct GameObject_Point* point_new(int x, int y, char character) {
    struct GameObject_Point *point = malloc(sizeof(struct GameObject_Point));
    if (point == NULL) return NULL;
    point_build(point, x, y, character);
    return point;
}

void point_build(struct GameObject_Point *point, int x, int y, char character) {
    vec2_build(&point->position, x, y);
    point->character = character;
    point->fg = COLOR_DEFAULT;
    point->bg = COLOR_DEFAULT;
}

void point_free(struct GameObject_Point *point) {
    if (point != NULL) {
        free(point);
    }
}

struct Vector2 point_get_position(struct GameObject_Point *point) {
    return point->position;
}

void point_set_position(struct GameObject_Point *point, struct Vector2 newPosition) {
    point->position = newPosition;
}

char point_get_character(struct GameObject_Point *point) {
    return point->character;
}

void point_set_character(struct GameObject_Point *point, char newCharacter) {
    point->character = newCharacter;
}

void point_set_color(struct GameObject_Point *point, Color fg, Color bg) {
    if (point == NULL) return;
    point->fg = fg;
    point->bg = bg;
}

Color point_get_fg(struct GameObject_Point *point) {
    if (point == NULL) return COLOR_DEFAULT;
    return point->fg;
}

Color point_get_bg(struct GameObject_Point *point) {
    if (point == NULL) return COLOR_DEFAULT;
    return point->bg;
}