#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/levelworld/levelworld.h"
#include "src/gameobject/player/player.h"
#include "src/gameobject/rectangle/rectangle.h"
#include "src/gameobject/circle/circle.h"
#include "src/gameobject/point/point.h"
#include "src/gameobject/text/text.h"
#include "src/collision/collision.h"
#include "src/color/color.h"
#include <stdio.h>

struct Engine *g_engine = NULL;
struct LevelWorld *g_world = NULL;
struct GameObject_Player *g_player = NULL;
struct GameObject_Point *g_gems[8];
int g_gems_collected = 0;

void update(struct Engine *engine, float dt) {
    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    float dx = 0.0f;
    float dy = 0.0f;

    if (input_is_key_down(KEY_UP) || input_is_key_down('z') || input_is_key_down('w')) dy -= 1.0f;
    if (input_is_key_down(KEY_DOWN) || input_is_key_down('s')) dy += 1.0f;
    if (input_is_key_down(KEY_LEFT) || input_is_key_down('q') || input_is_key_down('a')) dx -= 1.0f;
    if (input_is_key_down(KEY_RIGHT) || input_is_key_down('d')) dx += 1.0f;

    player_move(g_player, dx, dy, dt);
    collision_clamp_player(level_world_as_level(g_world), g_player);

    level_world_follow_player(g_world, g_player, 6.0f, dt);

    struct Level *base_lvl = level_world_as_level(g_world);

    if (input_is_key_down(KEY_SPACE)) {
        level_spawn_particles_sparkle(base_lvl, g_player->exact_x, g_player->exact_y, 2, COLOR_BRIGHT_YELLOW);
    }

    for (int i = 0; i < 8; i++) {
        if (g_gems[i] != NULL && collision_check_player_point(g_player, g_gems[i])) {
            level_spawn_particles_explosion(base_lvl, (float)g_gems[i]->position.x, (float)g_gems[i]->position.y, 12, COLOR_BRIGHT_CYAN);
            g_gems[i]->position.x = -100;
            g_gems[i]->position.y = -100;
            g_gems_collected++;
        }
    }

    level_update(base_lvl, dt);

    char top_buf[100];
    snprintf(top_buf, sizeof(top_buf), " PLAYER: (%3.0f, %2.0f) | CAM: (%3.0f, %2.0f) | GEMS: %d/8 | WORLD: 140x45 ",
             g_player->exact_x, g_player->exact_y, g_world->base.cam_x, g_world->base.cam_y, g_gems_collected);
    level_set_hud_text(base_lvl, HUD_TOP, 0, top_buf, COLOR_BRIGHT_GREEN, COLOR_BLACK);
}

int main() {
    g_world = level_world_new("The Great Overworld", 140, 45, 60, 16, '.');
    struct Level *base_lvl = level_world_as_level(g_world);
    level_set_default_color(base_lvl, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);

    level_set_hud_separator(base_lvl, HUD_TOP, '=', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_set_hud_text(base_lvl, HUD_TOP, 0, " EXPLORE THE VAST WORLD ", COLOR_BRIGHT_GREEN, COLOR_BLACK);

    level_set_hud_separator(base_lvl, HUD_BOTTOM, '=', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_set_hud_text(base_lvl, HUD_BOTTOM, 0, " [ZQSD / Arrows] Move  |  [SPACE] Trail  |  [ESC] Exit ", COLOR_WHITE, COLOR_BLUE);

    struct GameObject_Rectangle *wall1 = rectangle_new(0, 0, 140, 1, '#');
    rectangle_enable_filled(wall1);
    rectangle_set_color(wall1, COLOR_WHITE, COLOR_DEFAULT);
    level_add_rectangle(base_lvl, wall1);

    struct GameObject_Rectangle *wall2 = rectangle_new(0, 44, 140, 1, '#');
    rectangle_enable_filled(wall2);
    rectangle_set_color(wall2, COLOR_WHITE, COLOR_DEFAULT);
    level_add_rectangle(base_lvl, wall2);

    struct GameObject_Rectangle *wall3 = rectangle_new(0, 0, 1, 45, '#');
    rectangle_enable_filled(wall3);
    rectangle_set_color(wall3, COLOR_WHITE, COLOR_DEFAULT);
    level_add_rectangle(base_lvl, wall3);

    struct GameObject_Rectangle *wall4 = rectangle_new(139, 0, 1, 45, '#');
    rectangle_enable_filled(wall4);
    rectangle_set_color(wall4, COLOR_WHITE, COLOR_DEFAULT);
    level_add_rectangle(base_lvl, wall4);

    struct GameObject_Rectangle *fortress = rectangle_new(25, 10, 20, 8, 'H');
    rectangle_set_color(fortress, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_add_rectangle(base_lvl, fortress);

    struct GameObject_Rectangle *ruins = rectangle_new(75, 20, 25, 10, '=');
    rectangle_set_color(ruins, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_rectangle(base_lvl, ruins);

    struct GameObject_Ellipse *lake = circle_new(105, 10, 14, '~');
    ellipse_enable_filled(lake);
    ellipse_set_color(lake, COLOR_BRIGHT_BLUE, COLOR_BLUE);
    level_add_ellipse(base_lvl, lake);

    struct GameObject_Ellipse *grove = circle_new(50, 32, 12, 'T');
    ellipse_set_color(grove, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    level_add_ellipse(base_lvl, grove);

    struct GameObject_Text *sign1 = text_new_colored(28, 12, "RED FORTRESS", COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_add_text(base_lvl, sign1);

    struct GameObject_Text *sign2 = text_new_colored(105, 10, "BLUE LAKE", COLOR_BRIGHT_CYAN, COLOR_BLUE);
    level_add_text(base_lvl, sign2);

    struct GameObject_Text *sign3 = text_new_colored(80, 22, "ANCIENT RUINS", COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_text(base_lvl, sign3);

    int gem_coords[8][2] = {
        {10, 5}, {35, 14}, {60, 8}, {110, 12},
        {20, 35}, {55, 34}, {85, 25}, {130, 40}
    };

    for (int i = 0; i < 8; i++) {
        g_gems[i] = point_new(gem_coords[i][0], gem_coords[i][1], '$');
        point_set_color(g_gems[i], COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
        level_add_point(base_lvl, g_gems[i]);
    }

    g_player = player_new(12, 10, '@', 18.0f);
    player_set_color(g_player, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    level_add_player(base_lvl, g_player);

    g_engine = engine_new(base_lvl, 60, 16);
    engine_set_update_callback(g_engine, update);

    engine_run(g_engine, 30);

    engine_free(g_engine);
    return 0;
}
