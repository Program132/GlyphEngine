#ifndef CIRCLE_H
#define CIRCLE_H

#include "../ellipse/ellipse.h"

struct GameObject_Ellipse* circle_new(int x, int y, int diameter, char character);
struct GameObject_Ellipse* circle_new_textured(int x, int y, int diameter, struct Texture *texture);
void circle_build(struct GameObject_Ellipse *circle, int x, int y, int diameter, char character);

#endif
