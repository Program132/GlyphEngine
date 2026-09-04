#include "vec2.h"

void vec2_null(struct Vector2 *vec2) {
    vec2->x = 0;
    vec2->y = 0;
}

struct Vector2* vec2_new(int x, int y) {
    struct Vector2 *vec2 = malloc(sizeof(struct Vector2));
    if (vec2 == NULL) return NULL;
    vec2_build(vec2, x, y);
    return vec2;
}

void vec2_build(struct Vector2 *vec2, int x, int y) {
    vec2->x = x;
    vec2->y = y;
}

void vec2_setX(struct Vector2 *vec2, int x) {
    vec2->x = x;
}

void vec2_setY(struct Vector2 *vec2, int y) {
    vec2->y = y;
}

int vec2_getX(struct Vector2 *vec2) {
    return vec2->x;
}
int vec2_getY(struct Vector2 *vec2) {
    return vec2->y;
}

char* vec2_str(struct Vector2 *vec2) {
    char *result = malloc(50);
    if (result == NULL) return NULL;

    sprintf(result, "(x: %d, y: %d)", vec2->x, vec2->y);

    return result;
}

void vec2_free(struct Vector2 *vec2) {
    free(vec2);
}