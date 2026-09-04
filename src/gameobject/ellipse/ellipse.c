#include "ellipse.h"
#include <stdlib.h>
#include <stdbool.h>

struct GameObject_Ellipse* ellipse_new(int x, int y, int width, int height, char character) {
    struct GameObject_Ellipse* ellipse = malloc(sizeof(struct GameObject_Ellipse));
    if (ellipse == NULL) {
        return NULL;
    }
    ellipse_build(ellipse, x, y, width, height, character);
    return ellipse;
}

void ellipse_build(struct GameObject_Ellipse *ellipse, int x, int y, int width, int height, char character) {
    vec2_build(&ellipse->position, x, y);
    ellipse->width = width;
    ellipse->height = height;
    ellipse->character = character;
    ellipse->filled = 0;
}

void ellipse_free(struct GameObject_Ellipse *ellipse) {
    free(ellipse);
}

struct Vector2 ellipse_get_position(struct GameObject_Ellipse *ellipse) {
    return ellipse->position;
}

void ellipse_set_position(struct GameObject_Ellipse *ellipse, struct Vector2 newPosition) {
    ellipse->position = newPosition;
}

int ellipse_get_width(struct GameObject_Ellipse *ellipse) {
    return ellipse->width;
}

void ellipse_set_width(struct GameObject_Ellipse *ellipse, int newWidth) {
    ellipse->width = newWidth;
}

int ellipse_get_height(struct GameObject_Ellipse *ellipse) {
    return ellipse->height;
}

void ellipse_set_height(struct GameObject_Ellipse *ellipse, int newHeight) {
    ellipse->height = newHeight;
}

char ellipse_get_character(struct GameObject_Ellipse *ellipse) {
    return ellipse->character;
}

void ellipse_set_character(struct GameObject_Ellipse *ellipse, char newCharacter) {
    ellipse->character = newCharacter;
}

void ellipse_enable_filled(struct GameObject_Ellipse *ellipse) {
    ellipse->filled = 1;
}

void ellipse_disable_filled(struct GameObject_Ellipse *ellipse) {
    ellipse->filled = 0;
}

struct GameObject_Point* ellipse_get_points(struct GameObject_Ellipse *ellipse) {
    if (ellipse == NULL || ellipse->width <= 0 || ellipse->height <= 0) {
        return NULL;
    }

    struct GameObject_Point *points = (struct GameObject_Point*)malloc(ellipse->width * ellipse->height * sizeof(struct GameObject_Point));
    if (points == NULL) {
        return NULL;
    }

    double cx = ellipse->position.x + (ellipse->width - 1) / 2.0;
    double cy = ellipse->position.y + (ellipse->height - 1) / 2.0;
    double rx = ellipse->width / 2.0;
    double ry = ellipse->height / 2.0;

    int index = 0;
    for (int dy = 0; dy < ellipse->height; dy++) {
        for (int dx = 0; dx < ellipse->width; dx++) {
            double x = ellipse->position.x + dx;
            double y = ellipse->position.y + dy;

            double dx_norm = (x - cx) / rx;
            double dy_norm = (y - cy) / ry;
            double val = dx_norm * dx_norm + dy_norm * dy_norm;
            
            bool inside = val <= 1.0;

            bool border = val >= 0.5 && val <= 1.2;

            if ((ellipse->filled && inside) || (!ellipse->filled && border && inside)) {
                point_build(&points[index], x, y, ellipse->character);
                index++;
            }
        }
    }
    
    return points;
}
