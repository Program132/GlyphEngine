#include "../src/engine/engine.h"
#include "../src/inputs/inputs.h"
#include "../src/gameobject/player/player.h"
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

    if (player->position.x < 0) player_set_position(player, 0, player->position.y);
    if (player->position.y < 0) player_set_position(player, player->position.x, 0);
    if (player->position.x > engine->width - 3) player_set_position(player, engine->width - 3, player->position.y);
    if (player->position.y > engine->height - 3) player_set_position(player, player->position.x, engine->height - 3);
}

int main() {
    struct Level *level = level_new("Textured Player Demo", 50, 20, ' ');
    struct Engine *engine = engine_new(level, 50, 20);

    const char *sprite = 
        " o \n"
        "/|\\\n"
        "/ \\";
    struct Texture *tex = texture_new(3, 3, sprite, ' ');

    player = player_new_textured(23, 8, tex, 14.0f);
    level_add_player(level, player);

    engine_set_update_callback(engine, update);
    engine_run(engine, 30);

    engine_free(engine);

    return 0;
}
