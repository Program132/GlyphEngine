#include "square.h"
#include <stdlib.h>

struct GameObject_Rectangle* square_new(int x, int y, int size, char character) {
    return rectangle_new(x, y, size, size, character);
}

struct GameObject_Rectangle* square_new_textured(int x, int y, int size, struct Texture *texture) {
    struct GameObject_Rectangle* sq = rectangle_new(x, y, size, size, '\0');
    if (sq != NULL) {
        sq->texture = texture;
    }
    return sq;
}

void square_build(struct GameObject_Rectangle *square, int x, int y, int size, char character) {
    rectangle_build(square, x, y, size, size, character);
}
