#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../point/point.h"

struct GameObject_Rectangle {
    struct Vector2 position;
    int width;
    int height;
    char character;
    int filled;
};

struct GameObject_Rectangle* rectangle_new(int x, int y, int width, int height, char character);
void rectangle_build(struct GameObject_Rectangle *rectangle, int x, int y, int width, int height, char character);
void rectangle_free(struct GameObject_Rectangle *rectangle);
struct Vector2 rectangle_get_position(struct GameObject_Rectangle *rectangle);
void rectangle_set_position(struct GameObject_Rectangle *rectangle, struct Vector2 newPosition);
int rectangle_get_width(struct GameObject_Rectangle *rectangle);
void rectangle_set_width(struct GameObject_Rectangle *rectangle, int newWidth);
int rectangle_get_height(struct GameObject_Rectangle *rectangle);
void rectangle_set_height(struct GameObject_Rectangle *rectangle, int newHeight);
char rectangle_get_character(struct GameObject_Rectangle *rectangle);
void rectangle_set_character(struct GameObject_Rectangle *rectangle, char newCharacter);
void rectangle_enable_filled(struct GameObject_Rectangle *rectangle);
void rectangle_disable_filled(struct GameObject_Rectangle *rectangle);
struct GameObject_Point* rectangle_get_points(struct GameObject_Rectangle *rectangle);

#endif