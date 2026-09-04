#ifndef CIRCLE_H
#define CIRCLE_H

#include "../ellipse/ellipse.h"

struct GameObject_Ellipse* circle_new(int x, int y, int diameter, char character);
void circle_build(struct GameObject_Ellipse *circle, int x, int y, int diameter, char character);

#endif
