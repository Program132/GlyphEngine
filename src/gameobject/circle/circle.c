#include "circle.h"
#include <stdlib.h>

struct GameObject_Ellipse* circle_new(int x, int y, int diameter, char character) {
    return ellipse_new(x, y, diameter, diameter, character);
}

struct GameObject_Ellipse* circle_new_textured(int x, int y, int diameter, struct Texture *texture) {
    return ellipse_new_textured(x, y, diameter, diameter, texture);
}

void circle_build(struct GameObject_Ellipse *circle, int x, int y, int diameter, char character) {
    ellipse_build(circle, x, y, diameter, diameter, character);
}
