#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
    int width;
    int height;
    char *pixels;
    char transparent_char;
};

struct Texture* texture_new(int width, int height, const char *data, char transparent_char);
void texture_build(struct Texture *texture, int width, int height, const char *data, char transparent_char);
void texture_free(struct Texture *texture);
char texture_get_pixel(struct Texture *texture, int x, int y);
void texture_set_pixel(struct Texture *texture, int x, int y, char c);

#endif
