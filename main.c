#include "src/engine/engine.h"
#include "src/gameobject/square/square.h"
#include "src/gameobject/circle/circle.h"
#include "src/inputs/inputs.h"
#include <stdio.h>

struct GameObject_Rectangle* my_square;
struct GameObject_Ellipse* my_circle;

float sq_x = 2.0f, sq_y = 2.0f;
float sq_dx = 15.0f, sq_dy = 10.0f;

float ci_x = 25.0f, ci_y = 15.0f;
float ci_dx = -12.0f, ci_dy = -15.0f;

void update_shapes(struct Engine* engine, float delta_time) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    sq_x += sq_dx * delta_time;
    sq_y += sq_dy * delta_time;
    if (sq_x <= 0 || sq_x >= engine->width - 5) sq_dx = -sq_dx;
    if (sq_y <= 0 || sq_y >= engine->height - 5) sq_dy = -sq_dy;

    ci_x += ci_dx * delta_time;
    ci_y += ci_dy * delta_time;
    if (ci_x <= 0 || ci_x >= engine->width - 6) ci_dx = -ci_dx;
    if (ci_y <= 0 || ci_y >= engine->height - 6) ci_dy = -ci_dy;

    my_square->position.x = (int)sq_x;
    my_square->position.y = (int)sq_y;

    my_circle->position.x = (int)ci_x;
    my_circle->position.y = (int)ci_y;
}

int main() {
    struct Level* my_level = level_new("Rebounds", 50, 25, ' ');
    struct Engine* my_engine = engine_new(my_level, 50, 25);

    my_square = square_new((int)sq_x, (int)sq_y, 5, 'O');
    my_circle = circle_new((int)ci_x, (int)ci_y, 6, '@');

    level_add_rectangle(my_level, my_square);
    level_add_ellipse(my_level, my_circle);

    engine_set_update_callback(my_engine, update_shapes);
    engine_run(my_engine, 30);

    engine_free(my_engine);

    return 0;
}
