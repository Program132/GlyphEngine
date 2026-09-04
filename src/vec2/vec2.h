#ifndef VEC2_H
#define VEC2_H

#include <stdio.h>
#include <stdlib.h>

struct Vector2 {
    int x;
    int y;
};

void vec2_null(struct Vector2 *vec2);
struct Vector2* vec2_new(int x, int y);
void vec2_build(struct Vector2 *vec2, int x, int y);
void vec2_setX(struct Vector2 *vec2, int x);
void vec2_setY(struct Vector2 *vec2, int y);
int vec2_getX(struct Vector2 *vec2);
int vec2_getY(struct Vector2 *vec2);
char* vec2_str(struct Vector2 *vec2);
void vec2_free(struct Vector2 *vec2);

#endif