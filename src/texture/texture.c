#include "texture.h"
#include <stdlib.h>
#include <string.h>

struct Texture* texture_new(int width, int height, const char *data, char transparent_char) {
    struct Texture *texture = malloc(sizeof(struct Texture));
    if (texture == NULL) return NULL;
    texture_build(texture, width, height, data, transparent_char);
    return texture;
}

void texture_build(struct Texture *texture, int width, int height, const char *data, char transparent_char) {
    texture->width = width;
    texture->height = height;
    texture->transparent_char = transparent_char;
    texture->pixels = malloc(width * height + 1);
    if (texture->pixels == NULL) return;

    if (data == NULL) {
        memset(texture->pixels, transparent_char, width * height);
        texture->pixels[width * height] = '\0';
        return;
    }

    int data_idx = 0;
    int pixel_idx = 0;
    while (data[data_idx] != '\0' && pixel_idx < width * height) {
        char c = data[data_idx++];
        if (c == '\r' || c == '\n') continue;
        texture->pixels[pixel_idx++] = c;
    }

    while (pixel_idx < width * height) {
        texture->pixels[pixel_idx++] = transparent_char;
    }
    texture->pixels[width * height] = '\0';
}

void texture_free(struct Texture *texture) {
    if (texture == NULL) return;
    if (texture->pixels != NULL) {
        free(texture->pixels);
        texture->pixels = NULL;
    }
    free(texture);
}

char texture_get_pixel(struct Texture *texture, int x, int y) {
    if (texture == NULL || texture->pixels == NULL) return '\0';
    if (x < 0 || x >= texture->width || y < 0 || y >= texture->height) return texture->transparent_char;
    return texture->pixels[y * texture->width + x];
}

void texture_set_pixel(struct Texture *texture, int x, int y, char c) {
    if (texture == NULL || texture->pixels == NULL) return;
    if (x < 0 || x >= texture->width || y < 0 || y >= texture->height) return;
    texture->pixels[y * texture->width + x] = c;
}
