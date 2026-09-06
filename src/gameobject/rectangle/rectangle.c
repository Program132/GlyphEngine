#include "rectangle.h"
#include <stdlib.h>
#include <stdbool.h>

struct GameObject_Rectangle* rectangle_new(int x, int y, int width, int height, char character) {
    struct GameObject_Rectangle* rectangle = malloc(sizeof(struct GameObject_Rectangle));
    if (rectangle == NULL) {
        return NULL;
    }
    rectangle_build(rectangle, x, y, width, height, character);
    return rectangle;
}

struct GameObject_Rectangle* rectangle_new_textured(int x, int y, int width, int height, struct Texture *texture) {
    struct GameObject_Rectangle* rectangle = malloc(sizeof(struct GameObject_Rectangle));
    if (rectangle == NULL) {
        return NULL;
    }
    rectangle_build(rectangle, x, y, width, height, '\0');
    rectangle->texture = texture;
    return rectangle;
}

void rectangle_build(struct GameObject_Rectangle *rectangle, int x, int y, int width, int height, char character) {
    vec2_build(&rectangle->position, x, y);
    rectangle->width = width;
    rectangle->height = height;
    rectangle->character = character;
    rectangle->filled = 0;
    rectangle->texture = NULL;
    rectangle->fg = COLOR_DEFAULT;
    rectangle->bg = COLOR_DEFAULT;
}

void rectangle_free(struct GameObject_Rectangle *rectangle) {
    free(rectangle);
}

struct Vector2 rectangle_get_position(struct GameObject_Rectangle *rectangle) {
    return rectangle->position;
}

void rectangle_set_position(struct GameObject_Rectangle *rectangle, struct Vector2 newPosition) {
    rectangle->position = newPosition;
}

int rectangle_get_width(struct GameObject_Rectangle *rectangle) {
    return rectangle->width;
}

void rectangle_set_width(struct GameObject_Rectangle *rectangle, int newWidth) {
    rectangle->width = newWidth;
}

int rectangle_get_height(struct GameObject_Rectangle *rectangle) {
    return rectangle->height;
}

void rectangle_set_height(struct GameObject_Rectangle *rectangle, int newHeight) {
    rectangle->height = newHeight;
}

char rectangle_get_character(struct GameObject_Rectangle *rectangle) {
    return rectangle->character;
}

void rectangle_set_character(struct GameObject_Rectangle *rectangle, char newCharacter) {
    rectangle->character = newCharacter;
}

void rectangle_enable_filled(struct GameObject_Rectangle *rectangle) {
    rectangle->filled = 1;
}

void rectangle_disable_filled(struct GameObject_Rectangle *rectangle) {
    rectangle->filled = 0;
}

void rectangle_set_texture(struct GameObject_Rectangle *rectangle, struct Texture *texture) {
    if (rectangle == NULL) return;
    rectangle->texture = texture;
}

struct Texture* rectangle_get_texture(struct GameObject_Rectangle *rectangle) {
    if (rectangle == NULL) return NULL;
    return rectangle->texture;
}

void rectangle_set_color(struct GameObject_Rectangle *rectangle, Color fg, Color bg) {
    if (rectangle == NULL) return;
    rectangle->fg = fg;
    rectangle->bg = bg;
}

Color rectangle_get_fg(struct GameObject_Rectangle *rectangle) {
    if (rectangle == NULL) return COLOR_DEFAULT;
    return rectangle->fg;
}

Color rectangle_get_bg(struct GameObject_Rectangle *rectangle) {
    if (rectangle == NULL) return COLOR_DEFAULT;
    return rectangle->bg;
}

struct GameObject_Point* rectangle_get_points(struct GameObject_Rectangle *rectangle) {
    if (rectangle == NULL || rectangle->width <= 0 || rectangle->height <= 0) {
        return NULL;
    }

    int totalPoints;
    if (rectangle->filled || rectangle->width <= 2 || rectangle->height <= 2) {
        totalPoints = rectangle->width * rectangle->height;
    } else {
        totalPoints = 2 * rectangle->width + 2 * rectangle->height - 4;
    }

    struct GameObject_Point *points = (struct GameObject_Point*)malloc(totalPoints * sizeof(struct GameObject_Point));
    if (points == NULL) {
        return NULL;
    }

    int index = 0;
    for (int dy = 0; dy < rectangle->height; dy++) {
        for (int dx = 0; dx < rectangle->width; dx++) {
            bool is_border = (dx == 0 || dx == rectangle->width - 1 || dy == 0 || dy == rectangle->height - 1);

            if (rectangle->filled || is_border) {
                char ch = rectangle->character;
                if (rectangle->texture != NULL) {
                    ch = texture_get_pixel(rectangle->texture, dx % rectangle->texture->width, dy % rectangle->texture->height);
                }
                point_build(&points[index], rectangle->position.x + dx, rectangle->position.y + dy, ch);
                points[index].fg = rectangle->fg;
                points[index].bg = rectangle->bg;
                index++;
            }
        }
    }

    return points;
}