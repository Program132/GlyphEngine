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