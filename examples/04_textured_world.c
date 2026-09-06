#include "../src/engine/engine.h"
#include "../src/inputs/inputs.h"
#include "../src/gameobject/player/player.h"
#include "../src/gameobject/rectangle/rectangle.h"
#include "../src/texture/texture.h"

struct GameObject_Player *player;

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

    player_move(player, dx, dy, dt);

    if (player->position.x < 1) player_set_position(player, 1, player->position.y);
    if (player->position.y < 1) player_set_position(player, player->position.x, 1);
    if (player->position.x > engine->width - 4) player_set_position(player, engine->width - 4, player->position.y);
    if (player->position.y > engine->height - 4) player_set_position(player, player->position.x, engine->height - 4);
}

int main() {
    struct Level *level = level_new("Textured World Demo", 60, 22, ' ');
    struct Engine *engine = engine_new(level, 60, 22);

    const char *brick_pattern = 
        "[=][=]\n"
        "=[==]=";
    struct Texture *brick_tex = texture_new(6, 2, brick_pattern, ' ');

    const char *grass_pattern =
        "vv\"\"\n"
        "....";
    struct Texture *grass_tex = texture_new(4, 2, grass_pattern, ' ');

    struct GameObject_Rectangle *ground = rectangle_new(0, 18, 60, 4, '=');
    rectangle_enable_filled(ground);
    rectangle_set_texture(ground, grass_tex);
    level_add_rectangle(level, ground);

    struct GameObject_Rectangle *platform = rectangle_new(15, 11, 30, 2, '=');
    rectangle_enable_filled(platform);
    rectangle_set_texture(platform, brick_tex);
    level_add_rectangle(level, platform);

    struct GameObject_Rectangle *pillar = rectangle_new(4, 6, 6, 12, '#');
    rectangle_enable_filled(pillar);
    rectangle_set_texture(pillar, brick_tex);
    level_add_rectangle(level, pillar);

    const char *robot_skin = 
        "[o]\n"
        "/|\\\n"
        "d b";
    struct Texture *robot_tex = texture_new(3, 3, robot_skin, ' ');

    player = player_new_textured(28, 7, robot_tex, 12.0f);
    level_add_player(level, player);

    engine_set_update_callback(engine, update);
    engine_run(engine, 30);

    texture_free(brick_tex);
    texture_free(grass_tex);
    engine_free(engine);

    return 0;
}
