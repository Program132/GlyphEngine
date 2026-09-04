#ifndef SQUARE_H
#define SQUARE_H

#include "../rectangle/rectangle.h"

struct GameObject_Rectangle* square_new(int x, int y, int size, char character);
void square_build(struct GameObject_Rectangle *square, int x, int y, int size, char character);

#endif
