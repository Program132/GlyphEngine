#ifndef ELLIPSE_H
#define ELLIPSE_H

#include "../point/point.h"

struct GameObject_Ellipse {
    struct Vector2 position;
    int width;
    int height;
    char character;
    int filled;
};

struct GameObject_Ellipse* ellipse_new(int x, int y, int width, int height, char character);
void ellipse_build(struct GameObject_Ellipse *ellipse, int x, int y, int width, int height, char character);
void ellipse_free(struct GameObject_Ellipse *ellipse);
struct Vector2 ellipse_get_position(struct GameObject_Ellipse *ellipse);
void ellipse_set_position(struct GameObject_Ellipse *ellipse, struct Vector2 newPosition);
int ellipse_get_width(struct GameObject_Ellipse *ellipse);
void ellipse_set_width(struct GameObject_Ellipse *ellipse, int newWidth);
int ellipse_get_height(struct GameObject_Ellipse *ellipse);
void ellipse_set_height(struct GameObject_Ellipse *ellipse, int newHeight);
char ellipse_get_character(struct GameObject_Ellipse *ellipse);
void ellipse_set_character(struct GameObject_Ellipse *ellipse, char newCharacter);
void ellipse_enable_filled(struct GameObject_Ellipse *ellipse);
void ellipse_disable_filled(struct GameObject_Ellipse *ellipse);
struct GameObject_Point* ellipse_get_points(struct GameObject_Ellipse *ellipse);

#endif
