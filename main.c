#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/gameobject/player/player.h"
#include "src/gameobject/rectangle/rectangle.h"
#include "src/gameobject/circle/circle.h"
#include "src/gameobject/point/point.h"
#include "src/gameobject/text/text.h"
#include "src/collision/collision.h"
#include "src/color/color.h"
#include "src/texture/texture.h"
#include <stdio.h>

struct GameObject_Player *player;
struct GameObject_Rectangle *wall_left;
struct GameObject_Rectangle *wall_right;
struct GameObject_Rectangle *lava_pool;
struct GameObject_Ellipse *orb;
struct GameObject_Point *crystal;
struct GameObject_Text *hud_text;
struct GameObject_Text *banner_text;

float orb_x = 20.0f;
float orb_y = 6.0f;
float orb_dx = 10.0f;
int score = 0;

void update(struct Engine *engine, float dt) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    if (!player_is_alive(player)) {
        text_set_content(banner_text, "=== GAME OVER - Press ESC ===");
        text_set_color(banner_text, COLOR_BRIGHT_RED, COLOR_DEFAULT);
        player_set_color(player, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);
        return;
    }

    float old_x = player->exact_x;
    float old_y = player->exact_y;
    int old_px = player->position.x;
    int old_py = player->position.y;

    float dx = 0.0f;
    float dy = 0.0f;

    if (input_is_key_down(KEY_Z) || input_is_key_down('z') || input_is_key_down('Z') || input_is_key_down(KEY_UP) || input_is_key_down('w') || input_is_key_down('W')) dy -= 1.0f;
    if (input_is_key_down(KEY_S) || input_is_key_down('s') || input_is_key_down('S') || input_is_key_down(KEY_DOWN)) dy += 1.0f;
    if (input_is_key_down(KEY_Q) || input_is_key_down('q') || input_is_key_down('Q') || input_is_key_down(KEY_LEFT) || input_is_key_down('a') || input_is_key_down('A')) dx -= 1.0f;
    if (input_is_key_down(KEY_D) || input_is_key_down('d') || input_is_key_down('D') || input_is_key_down(KEY_RIGHT)) dx += 1.0f;

    player_move(player, dx, dy, dt);

    if (collision_check_player_rect(player, wall_left) ||
        collision_check_player_rect(player, wall_right)) {
        player->exact_x = old_x;
        player->exact_y = old_y;
        player->position.x = old_px;
        player->position.y = old_py;
    }

    collision_clamp_player(engine->current_level, player);

    orb_x += orb_dx * dt;
    if (orb_x <= 15.0f || orb_x >= 45.0f) orb_dx = -orb_dx;
    orb->position.x = (int)orb_x;

    if (collision_check_player_ellipse(player, orb)) {
        player_take_damage(player, 1);
        player_set_color(player, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    } else if (collision_check_player_rect(player, lava_pool)) {
        player_take_damage(player, 2);
        player_set_color(player, COLOR_RED, COLOR_DEFAULT);
    } else {
        player_set_color(player, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    }

    if (crystal != NULL && collision_check_player_point(player, crystal)) {
        player_heal(player, 25);
        score += 10;
        crystal->position.x = 12 + (crystal->position.x * 7) % 36;
        crystal->position.y = 5 + (crystal->position.y * 3) % 8;
    }

    int hp = player_get_health(player);
    char buf[64];
    snprintf(buf, sizeof(buf), "HP: %3d/100  |  SCORE: %4d", hp, score);
    text_set_content(hud_text, buf);
    if (hp > 50) {
        text_set_color(hud_text, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    } else if (hp > 20) {
        text_set_color(hud_text, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    } else {
        text_set_color(hud_text, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    }
}

int main() {
    struct Level *level = level_new("Collisions & Colors", 60, 20, '.');
    level_set_default_color(level, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);

    struct Engine *engine = engine_new(level, 60, 20);

    hud_text = text_new_colored(2, 0, "HP: 100/100  |  SCORE:    0", COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    level_add_text(level, hud_text);

    banner_text = text_new_colored(16, 18, "", COLOR_WHITE, COLOR_DEFAULT);
    level_add_text(level, banner_text);

    wall_left = rectangle_new(8, 4, 3, 12, '#');
    rectangle_enable_filled(wall_left);
    rectangle_set_color(wall_left, COLOR_BLUE, COLOR_DEFAULT);
    level_add_rectangle(level, wall_left);

    wall_right = rectangle_new(50, 4, 3, 12, '#');
    rectangle_enable_filled(wall_right);
    rectangle_set_color(wall_right, COLOR_BLUE, COLOR_DEFAULT);
    level_add_rectangle(level, wall_right);

    lava_pool = rectangle_new(22, 14, 16, 3, '~');
    rectangle_enable_filled(lava_pool);
    rectangle_set_color(lava_pool, COLOR_BRIGHT_RED, COLOR_RED);
    level_add_rectangle(level, lava_pool);

    orb = circle_new((int)orb_x, (int)orb_y, 6, 'O');
    ellipse_enable_filled(orb);
    ellipse_set_color(orb, COLOR_BRIGHT_MAGENTA, COLOR_DEFAULT);
    level_add_ellipse(level, orb);

    crystal = point_new(30, 8, '*');
    point_set_color(crystal, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_point(level, crystal);

    const char *ship =
        " ^ \n"
        "<#>\n"
        "v v";
    struct Texture *ship_tex = texture_new(3, 3, ship, ' ');

    player = player_new_textured(28, 2, ship_tex, 14.0f);
    player_set_color(player, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    level_add_player(level, player);

    engine_set_update_callback(engine, update);
    engine_run(engine, 30);

    texture_free(ship_tex);
    engine_free(engine);

    return 0;
}
