#include "../src/engine/engine.h"
#include "../src/inputs/inputs.h"
#include "../src/gameobject/player/player.h"
#include "../src/gameobject/rectangle/rectangle.h"
#include "../src/gameobject/point/point.h"
#include "../src/texture/texture.h"

struct GameObject_Player *hero;
struct GameObject_Point *coin;

void update(struct Engine *engine, float dt) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    float dx = 0.0f;
    float dy = 0.0f;

    if (input_is_key_down(KEY_Z) || input_is_key_down(KEY_UP)) dy -= 1.0f;
    if (input_is_key_down(KEY_S) || input_is_key_down(KEY_DOWN)) dy += 1.0f;
    if (input_is_key_down(KEY_Q) || input_is_key_down(KEY_LEFT)) dx -= 1.0f;
    if (input_is_key_down(KEY_D) || input_is_key_down(KEY_RIGHT)) dx += 1.0f;

    player_move(hero, dx, dy, dt);

    if (hero->position.x < 3) player_set_position(hero, 3, hero->position.y);
    if (hero->position.y < 3) player_set_position(hero, hero->position.x, 3);
    if (hero->position.x > engine->width - 5) player_set_position(hero, engine->width - 5, hero->position.y);
    if (hero->position.y > engine->height - 5) player_set_position(hero, hero->position.x, engine->height - 5);

    if (coin != NULL && hero->position.x == coin->position.x && hero->position.y == coin->position.y) {
        player_heal(hero, 10);
        coin->position.x = 8 + (hero->position.x * 7) % 30;
        coin->position.y = 5 + (hero->position.y * 3) % 10;
    }
}

int main() {
    struct Level *level = level_new("Dungeon Crawler", 50, 20, '.');
    struct Engine *engine = engine_new(level, 50, 20);

    const char *wall_pattern =
        "####\n"
        "#--#\n"
        "#--#\n"
        "####";
    struct Texture *wall_tex = texture_new(4, 4, wall_pattern, ' ');

    struct GameObject_Rectangle *top_wall = rectangle_new(0, 0, 50, 2, '#');
    rectangle_enable_filled(top_wall);
    rectangle_set_texture(top_wall, wall_tex);
    level_add_rectangle(level, top_wall);

    struct GameObject_Rectangle *bottom_wall = rectangle_new(0, 18, 50, 2, '#');
    rectangle_enable_filled(bottom_wall);
    rectangle_set_texture(bottom_wall, wall_tex);
    level_add_rectangle(level, bottom_wall);

    struct GameObject_Rectangle *left_wall = rectangle_new(0, 0, 2, 20, '#');
    rectangle_enable_filled(left_wall);
    rectangle_set_texture(left_wall, wall_tex);
    level_add_rectangle(level, left_wall);

    struct GameObject_Rectangle *right_wall = rectangle_new(48, 0, 2, 20, '#');
    rectangle_enable_filled(right_wall);
    rectangle_set_texture(right_wall, wall_tex);
    level_add_rectangle(level, right_wall);

    coin = point_new(25, 10, '$');
    level_add_point(level, coin);

    const char *knight_art =
        "(+)\n"
        "/|\\\n"
        "/ \\";
    struct Texture *knight_tex = texture_new(3, 3, knight_art, ' ');

    hero = player_new_textured(10, 10, knight_tex, 15.0f);
    player_set_health(hero, 80);
    level_add_player(level, hero);

    engine_set_update_callback(engine, update);
    engine_run(engine, 30);

    texture_free(wall_tex);
    engine_free(engine);

    return 0;
}
