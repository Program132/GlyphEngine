#include "square.h"
#include <stdlib.h>

struct GameObject_Rectangle* square_new(int x, int y, int size, char character) {
    return rectangle_new(x, y, size, size, character);
}

void square_build(struct GameObject_Rectangle *square, int x, int y, int size, char character) {
    rectangle_build(square, x, y, size, size, character);
}
