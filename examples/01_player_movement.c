#include "../src/engine/engine.h"
#include "../src/gameobject/point/point.h"
#include "../src/gameobject/rectangle/rectangle.h"
#include "../src/inputs/inputs.h"

struct GameObject_Point* player;
float player_x = 5.0f;
float player_y = 5.0f;
float speed = 15.0f;

void my_update(struct Engine* engine, float delta_time) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    if (input_is_key_down(KEY_Z) || input_is_key_down(KEY_UP))    player_y -= speed * delta_time;
    if (input_is_key_down(KEY_S) || input_is_key_down(KEY_DOWN))  player_y += speed * delta_time;
    if (input_is_key_down(KEY_Q) || input_is_key_down(KEY_LEFT))  player_x -= speed * delta_time;
    if (input_is_key_down(KEY_D) || input_is_key_down(KEY_RIGHT)) player_x += speed * delta_time;

    if (player_x < 0) player_x = 0;
    if (player_y < 0) player_y = 0;
    if (player_x >= 39) player_x = 39;
    if (player_y >= 19) player_y = 19;

    player->position.x = (int)player_x;
    player->position.y = (int)player_y;
}

int main() {
    struct Level* my_level = level_new("Mon Labyrinthe", 40, 20, '.');
    struct Engine* my_engine = engine_new(my_level, 40, 20);

    player = point_new((int)player_x, (int)player_y, 'P');
    level_add_point(my_level, player);

    struct GameObject_Rectangle* wall = rectangle_new(15, 5, 5, 10, '#');
    rectangle_enable_filled(wall);
    level_add_rectangle(my_level, wall);

    engine_set_update_callback(my_engine, my_update);
    engine_run(my_engine, 30);

    engine_free(my_engine);

    return 0;
}
