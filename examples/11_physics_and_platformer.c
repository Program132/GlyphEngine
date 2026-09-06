#include "../src/engine/engine.h"
#include "../src/inputs/inputs.h"
#include "../src/level/level.h"
#include "../src/gameobject/player/player.h"
#include "../src/gameobject/rectangle/rectangle.h"
#include "../src/gameobject/circle/circle.h"
#include "../src/gameobject/point/point.h"
#include "../src/physics/physics.h"
#include "../src/collision/collision.h"
#include "../src/color/color.h"
#include <stdio.h>

struct Engine *g_engine = NULL;
struct Level *g_level = NULL;
struct GameObject_Player *g_player = NULL;
struct GameObject_Rectangle *g_box = NULL;
struct PhysicsBody g_box_body;
struct GameObject_Ellipse *g_ball = NULL;
struct PhysicsBody g_ball_body;
struct GameObject_Point *g_coins[4];
int g_coins_collected = 0;

void update(struct Engine *engine, float dt) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    float move_x = 0.0f;
    if (input_is_key_down(KEY_LEFT) || input_is_key_down('q') || input_is_key_down('a')) move_x -= 1.0f;
    if (input_is_key_down(KEY_RIGHT) || input_is_key_down('d')) move_x += 1.0f;

    int jump_pressed = (input_is_key_down(KEY_SPACE) || input_is_key_down(KEY_UP) || input_is_key_down('z') || input_is_key_down('w'));

    physics_simulate_player(g_player, g_level, move_x, jump_pressed, dt);

    physics_simulate_rectangle(g_box, &g_box_body, g_level, dt);
    physics_simulate_ellipse(g_ball, &g_ball_body, g_level, dt);

    if (collision_check_player_rect(g_player, g_box)) {
        if (g_player->vx > 0.0f) {
            physics_body_apply_force(&g_box_body, 12.0f, -4.0f);
        } else if (g_player->vx < 0.0f) {
            physics_body_apply_force(&g_box_body, -12.0f, -4.0f);
        }
    }

    if (collision_check_player_ellipse(g_player, g_ball)) {
        if (g_player->vx > 0.0f) {
            physics_body_apply_force(&g_ball_body, 16.0f, -10.0f);
        } else if (g_player->vx < 0.0f) {
            physics_body_apply_force(&g_ball_body, -16.0f, -10.0f);
        } else {
            physics_body_apply_force(&g_ball_body, 0.0f, -14.0f);
        }
        level_spawn_particles_sparkle(g_level, (float)g_ball->position.x, (float)g_ball->position.y, 4, COLOR_BRIGHT_YELLOW);
    }

    for (int i = 0; i < 4; i++) {
        if (g_coins[i] != NULL && collision_check_player_point(g_player, g_coins[i])) {
            level_spawn_particles_explosion(g_level, (float)g_coins[i]->position.x, (float)g_coins[i]->position.y, 10, COLOR_BRIGHT_YELLOW);
            g_coins[i]->position.x = -100;
            g_coins[i]->position.y = -100;
            g_coins_collected++;
        }
    }

    level_update(g_level, dt);

    char hud_buf[100];
    snprintf(hud_buf, sizeof(hud_buf), " GROUNDED: %-3s | VY: %+5.1f | JUMP POWER: %2.0f | COINS: %d/4 ",
             player_is_grounded(g_player) ? "YES" : "NO", g_player->vy, player_get_jump_power(g_player), g_coins_collected);
    Color hud_col = player_is_grounded(g_player) ? COLOR_BRIGHT_GREEN : COLOR_BRIGHT_YELLOW;
    level_set_hud_text(g_level, HUD_TOP, 0, hud_buf, hud_col, COLOR_BLACK);
}

int main() {
    g_level = level_new("Platformer Physics Arena", 60, 18, '.');
    level_set_default_color(g_level, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);

    level_set_hud_separator(g_level, HUD_TOP, '=', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_set_hud_text(g_level, HUD_TOP, 0, " PLATFORMER PHYSICS READY ", COLOR_BRIGHT_GREEN, COLOR_BLACK);

    level_set_hud_separator(g_level, HUD_BOTTOM, '=', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_set_hud_text(g_level, HUD_BOTTOM, 0, " [A/D] Move  |  [SPACE/W] Jump  |  Push Box & Ball!  |  [ESC] Exit ", COLOR_WHITE, COLOR_BLUE);

    struct GameObject_Rectangle *ground = rectangle_new(0, 16, 60, 2, '=');
    rectangle_enable_filled(ground);
    rectangle_set_color(ground, COLOR_WHITE, COLOR_DEFAULT);
    rectangle_set_solid(ground, 1);
    level_add_rectangle(g_level, ground);

    struct GameObject_Rectangle *plat1 = rectangle_new(8, 12, 14, 2, '#');
    rectangle_enable_filled(plat1);
    rectangle_set_color(plat1, COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    rectangle_set_solid(plat1, 1);
    level_add_rectangle(g_level, plat1);

    struct GameObject_Rectangle *plat2 = rectangle_new(26, 8, 16, 2, '#');
    rectangle_enable_filled(plat2);
    rectangle_set_color(plat2, COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    rectangle_set_solid(plat2, 1);
    level_add_rectangle(g_level, plat2);

    struct GameObject_Rectangle *plat3 = rectangle_new(44, 11, 12, 2, '#');
    rectangle_enable_filled(plat3);
    rectangle_set_color(plat3, COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    rectangle_set_solid(plat3, 1);
    level_add_rectangle(g_level, plat3);

    struct GameObject_Rectangle *decor = rectangle_new(2, 2, 8, 3, ':');
    rectangle_set_color(decor, COLOR_BLUE, COLOR_DEFAULT);
    rectangle_set_solid(decor, 0);
    level_add_rectangle(g_level, decor);

    g_box = rectangle_new(32, 4, 4, 2, 'B');
    rectangle_enable_filled(g_box);
    rectangle_set_color(g_box, COLOR_BRIGHT_MAGENTA, COLOR_DEFAULT);
    rectangle_set_solid(g_box, 1);
    level_add_rectangle(g_level, g_box);
    physics_body_init(&g_box_body, 32.0f, 4.0f, 35.0f, 0.2f);

    g_ball = circle_new(48, 4, 3, 'O');
    ellipse_enable_filled(g_ball);
    ellipse_set_color(g_ball, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_ellipse(g_level, g_ball);
    physics_body_init(&g_ball_body, 48.0f, 4.0f, 35.0f, 0.75f);

    int coin_pos[4][2] = {{14, 10}, {34, 6}, {50, 9}, {55, 14}};
    for (int i = 0; i < 4; i++) {
        g_coins[i] = point_new(coin_pos[i][0], coin_pos[i][1], '$');
        point_set_color(g_coins[i], COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
        level_add_point(g_level, g_coins[i]);
    }

    g_player = player_new(4, 14, '@', 16.0f);
    player_set_color(g_player, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    player_enable_gravity(g_player, 40.0f);
    player_set_jump_power(g_player, 19.0f);
    level_add_player(g_level, g_player);

    g_engine = engine_new(g_level, 60, 18);
    engine_set_update_callback(g_engine, update);

    engine_run(g_engine, 30);

    engine_free(g_engine);
    return 0;
}
