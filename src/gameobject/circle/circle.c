#include "circle.h"
#include <stdlib.h>

struct GameObject_Ellipse* circle_new(int x, int y, int diameter, char character) {
    return ellipse_new(x, y, diameter, diameter, character);
}

void circle_build(struct GameObject_Ellipse *circle, int x, int y, int diameter, char character) {
    ellipse_build(circle, x, y, diameter, diameter, character);
}
