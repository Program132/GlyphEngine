#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/gameobject/circle/circle.h"
#include "src/gameobject/ellipse/ellipse.h"
#include "src/gameobject/square/square.h"
#include "src/texture/texture.h"

struct GameObject_Ellipse *orb;
struct GameObject_Ellipse *saucer;
struct GameObject_Rectangle *crate;

float orb_x = 4.0f, orb_y = 3.0f;
float orb_dx = 14.0f, orb_dy = 9.0f;

void update(struct Engine *engine, float dt) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    orb_x += orb_dx * dt;
    orb_y += orb_dy * dt;

    if (orb_x <= 1 || orb_x >= engine->width - 9) orb_dx = -orb_dx;
    if (orb_y <= 1 || orb_y >= engine->height - 9) orb_dy = -orb_dy;

    orb->position.x = (int)orb_x;
    orb->position.y = (int)orb_y;
}

int main() {
    struct Level *level = level_new("Textured Shapes Demo", 60, 22, ' ');
    struct Engine *engine = engine_new(level, 60, 22);

    const char *circle_pattern =
        "/*\\*\n"
        "*\\/*\n"
        "/*\\*\n"
        "*\\/*";
    struct Texture *orb_tex = texture_new(4, 4, circle_pattern, ' ');

    orb = circle_new_textured((int)orb_x, (int)orb_y, 8, orb_tex);
    ellipse_enable_filled(orb);
    level_add_ellipse(level, orb);

    const char *saucer_pattern =
        "====\n"
        "####\n"
        "====\n"
        "....";
    struct Texture *saucer_tex = texture_new(4, 4, saucer_pattern, ' ');

    saucer = ellipse_new_textured(35, 4, 18, 6, saucer_tex);
    ellipse_enable_filled(saucer);
    level_add_ellipse(level, saucer);

    const char *crate_pattern =
        "+--+\n"
        "|/\\|\n"
        "|\\/|\n"
        "+--+";
    struct Texture *crate_tex = texture_new(4, 4, crate_pattern, ' ');

    crate = square_new_textured(25, 14, 6, crate_tex);
    rectangle_enable_filled(crate);
    level_add_rectangle(level, crate);

    engine_set_update_callback(engine, update);
    engine_run(engine, 30);

    texture_free(orb_tex);
    texture_free(saucer_tex);
    texture_free(crate_tex);
    engine_free(engine);

    return 0;
}
