#include "../src/engine/engine.h"
#include "../src/inputs/inputs.h"
#include "../src/level/level.h"
#include "../src/gameobject/player/player.h"
#include "../src/gameobject/rectangle/rectangle.h"
#include "../src/gameobject/point/point.h"
#include "../src/gameobject/text/text.h"
#include "../src/color/color.h"
#include "../src/particle/particle.h"
#include "../src/light/light.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static struct Engine *g_engine = NULL;
static struct Level *g_level = NULL;
static struct GameObject_Player *g_hero = NULL;
static struct Light *g_player_torch = NULL;
static struct Light *g_torch_left = NULL;
static struct Light *g_torch_right = NULL;
static struct Light *g_torch_center = NULL;
static struct Light *g_crystal_cyan = NULL;
static struct Light *g_crystal_purple = NULL;
static struct Light *g_crystal_green = NULL;

static float g_hero_x = 40.0f;
static float g_hero_y = 15.0f;
static float g_speed = 15.0f;
static float g_time = 0.0f;

static int g_torch_active = 1;
static int g_lighting_enabled = 1;
static int g_ambient_mode = 0;
static int g_prev_f = 0;
static int g_prev_l = 0;
static int g_prev_c = 0;

static void update(struct Engine *engine, float dt) {
    if (dt > 0.04f) dt = 0.04f;
    g_time += dt;

    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    float move_x = 0.0f;
    float move_y = 0.0f;

    if (input_is_key_down(KEY_LEFT) || input_is_key_down('q') || input_is_key_down('a') || input_is_key_down('Q') || input_is_key_down('A')) move_x -= 1.0f;
    if (input_is_key_down(KEY_RIGHT) || input_is_key_down('d') || input_is_key_down('D')) move_x += 1.0f;
    if (input_is_key_down(KEY_UP) || input_is_key_down('z') || input_is_key_down('w') || input_is_key_down('Z') || input_is_key_down('W')) move_y -= 1.0f;
    if (input_is_key_down(KEY_DOWN) || input_is_key_down('s') || input_is_key_down('S')) move_y += 1.0f;

    if (move_x != 0.0f && move_y != 0.0f) {
        move_x *= 0.7071f;
        move_y *= 0.7071f;
    }

    float next_x = g_hero_x + move_x * g_speed * dt;
    float next_y = g_hero_y + move_y * g_speed * dt;

    int test_cell_x = (int)(next_x >= 0.0f ? next_x + 0.5f : next_x - 0.5f);
    int test_cell_y = (int)(next_y >= 0.0f ? next_y + 0.5f : next_y - 0.5f);

    if (!level_is_solid_at(g_level, test_cell_x, (int)g_hero_y)) {
        g_hero_x = next_x;
    }
    if (!level_is_solid_at(g_level, (int)g_hero_x, test_cell_y)) {
        g_hero_y = next_y;
    }

    player_set_position(g_hero, (int)g_hero_x, (int)g_hero_y);
    light_set_position(g_player_torch, g_hero_x, g_hero_y);

    if (fabsf(move_x) > 0.01f || fabsf(move_y) > 0.01f) {
        if (g_torch_active && ((rand() % 3) == 0)) {
            level_spawn_particles_sparkle(g_level, g_hero_x, g_hero_y, 1, color_rgb(255, 190, 70));
        }
    }

    if ((rand() % 8) == 0) level_spawn_particles_sparkle(g_level, 12.0f, 5.0f, 1, color_rgb(255, 140, 30));
    if ((rand() % 8) == 0) level_spawn_particles_sparkle(g_level, 67.0f, 5.0f, 1, color_rgb(255, 140, 30));
    if ((rand() % 8) == 0) level_spawn_particles_sparkle(g_level, 40.0f, 19.0f, 1, color_rgb(255, 140, 30));

    int key_f = input_is_key_down('f') || input_is_key_down('F');
    if (key_f && !g_prev_f) {
        g_torch_active = !g_torch_active;
        light_set_active(g_player_torch, g_torch_active);
    }
    g_prev_f = key_f;

    int key_l = input_is_key_down('l') || input_is_key_down('L');
    if (key_l && !g_prev_l) {
        g_lighting_enabled = !g_lighting_enabled;
        level_enable_lighting(g_level, g_lighting_enabled);
    }
    g_prev_l = key_l;

    int key_c = input_is_key_down('c') || input_is_key_down('C');
    if (key_c && !g_prev_c) {
        g_ambient_mode = (g_ambient_mode + 1) % 3;
        if (g_ambient_mode == 0) {
            level_set_ambient_light(g_level, color_rgb(18, 18, 30));
        } else if (g_ambient_mode == 1) {
            level_set_ambient_light(g_level, color_rgb(38, 38, 55));
        } else {
            level_set_ambient_light(g_level, color_rgb(3, 3, 6));
        }
    }
    g_prev_c = key_c;

    const char *ambient_names[3] = {"DUNGEON DARK", "DIM AMBIENT", "PITCH BLACK"};
    char status_str[100];
    snprintf(status_str, sizeof(status_str), " Torch [F]: %s  |  Shadows & Lights [L]: %s  |  Ambient [C]: %s",
             g_torch_active ? "ON " : "OFF",
             g_lighting_enabled ? "ON " : "OFF",
             ambient_names[g_ambient_mode]);

    level_set_hud_text(g_level, HUD_TOP, 1, status_str, color_rgb(255, 220, 120), COLOR_DEFAULT);
}

int main() {
    int width = 80;
    int height = 24;

    g_level = level_new("Dynamic Lighting & Shadows Dungeon", width, height, '.');
    level_set_default_color(g_level, color_rgb(60, 60, 80), color_rgb(12, 12, 20));

    level_enable_lighting(g_level, 1);
    level_set_ambient_light(g_level, color_rgb(18, 18, 30));

    struct GameObject_Rectangle *top_wall = rectangle_new(0, 0, width, 1, '#');
    rectangle_enable_filled(top_wall);
    rectangle_set_solid(top_wall, 1);
    rectangle_set_color(top_wall, color_rgb(130, 130, 150), color_rgb(20, 20, 30));
    level_add_rectangle(g_level, top_wall);

    struct GameObject_Rectangle *bottom_wall = rectangle_new(0, height - 1, width, 1, '#');
    rectangle_enable_filled(bottom_wall);
    rectangle_set_solid(bottom_wall, 1);
    rectangle_set_color(bottom_wall, color_rgb(130, 130, 150), color_rgb(20, 20, 30));
    level_add_rectangle(g_level, bottom_wall);

    struct GameObject_Rectangle *left_wall = rectangle_new(0, 0, 1, height, '#');
    rectangle_enable_filled(left_wall);
    rectangle_set_solid(left_wall, 1);
    rectangle_set_color(left_wall, color_rgb(130, 130, 150), color_rgb(20, 20, 30));
    level_add_rectangle(g_level, left_wall);

    struct GameObject_Rectangle *right_wall = rectangle_new(width - 1, 0, 1, height, '#');
    rectangle_enable_filled(right_wall);
    rectangle_set_solid(right_wall, 1);
    rectangle_set_color(right_wall, color_rgb(130, 130, 150), color_rgb(20, 20, 30));
    level_add_rectangle(g_level, right_wall);

    struct GameObject_Rectangle *p1 = rectangle_new(16, 9, 4, 3, '#');
    rectangle_enable_filled(p1);
    rectangle_set_solid(p1, 1);
    rectangle_set_color(p1, color_rgb(140, 140, 160), color_rgb(30, 30, 45));
    level_add_rectangle(g_level, p1);

    struct GameObject_Rectangle *p2 = rectangle_new(30, 7, 3, 4, '#');
    rectangle_enable_filled(p2);
    rectangle_set_solid(p2, 1);
    rectangle_set_color(p2, color_rgb(140, 140, 160), color_rgb(30, 30, 45));
    level_add_rectangle(g_level, p2);

    struct GameObject_Rectangle *p3 = rectangle_new(47, 7, 3, 4, '#');
    rectangle_enable_filled(p3);
    rectangle_set_solid(p3, 1);
    rectangle_set_color(p3, color_rgb(140, 140, 160), color_rgb(30, 30, 45));
    level_add_rectangle(g_level, p3);

    struct GameObject_Rectangle *p4 = rectangle_new(60, 9, 4, 3, '#');
    rectangle_enable_filled(p4);
    rectangle_set_solid(p4, 1);
    rectangle_set_color(p4, color_rgb(140, 140, 160), color_rgb(30, 30, 45));
    level_add_rectangle(g_level, p4);

    struct GameObject_Rectangle *p5 = rectangle_new(28, 16, 4, 3, '#');
    rectangle_enable_filled(p5);
    rectangle_set_solid(p5, 1);
    rectangle_set_color(p5, color_rgb(140, 140, 160), color_rgb(30, 30, 45));
    level_add_rectangle(g_level, p5);

    struct GameObject_Rectangle *p6 = rectangle_new(48, 16, 4, 3, '#');
    rectangle_enable_filled(p6);
    rectangle_set_solid(p6, 1);
    rectangle_set_color(p6, color_rgb(140, 140, 160), color_rgb(30, 30, 45));
    level_add_rectangle(g_level, p6);

    struct GameObject_Rectangle *altar = rectangle_new(38, 12, 4, 2, '=');
    rectangle_enable_filled(altar);
    rectangle_set_solid(altar, 1);
    rectangle_set_color(altar, color_rgb(180, 160, 120), color_rgb(40, 35, 25));
    level_add_rectangle(g_level, altar);

    g_torch_left = light_new(12.0f, 5.0f, 11.0f, 1.2f, color_rgb(255, 140, 30));
    light_set_flicker(g_torch_left, 0.22f, 12.0f);
    light_set_cast_shadows(g_torch_left, 1);
    level_add_light(g_level, g_torch_left);

    struct GameObject_Point *vis_torch_l = point_new(12, 5, '*');
    point_set_color(vis_torch_l, color_rgb(255, 220, 60), COLOR_DEFAULT);
    level_add_point(g_level, vis_torch_l);

    g_torch_right = light_new(67.0f, 5.0f, 11.0f, 1.2f, color_rgb(255, 140, 30));
    light_set_flicker(g_torch_right, 0.20f, 10.0f);
    light_set_cast_shadows(g_torch_right, 1);
    level_add_light(g_level, g_torch_right);

    struct GameObject_Point *vis_torch_r = point_new(67, 5, '*');
    point_set_color(vis_torch_r, color_rgb(255, 220, 60), COLOR_DEFAULT);
    level_add_point(g_level, vis_torch_r);

    g_torch_center = light_new(40.0f, 19.0f, 10.0f, 1.1f, color_rgb(255, 150, 40));
    light_set_flicker(g_torch_center, 0.18f, 9.0f);
    light_set_cast_shadows(g_torch_center, 1);
    level_add_light(g_level, g_torch_center);

    struct GameObject_Point *vis_torch_c = point_new(40, 19, '*');
    point_set_color(vis_torch_c, color_rgb(255, 220, 60), COLOR_DEFAULT);
    level_add_point(g_level, vis_torch_c);

    g_crystal_cyan = light_new(16.0f, 18.0f, 11.0f, 1.25f, color_rgb(20, 220, 255));
    light_set_cast_shadows(g_crystal_cyan, 1);
    level_add_light(g_level, g_crystal_cyan);

    struct GameObject_Point *vis_c_cyan = point_new(16, 18, '+');
    point_set_color(vis_c_cyan, color_rgb(120, 240, 255), COLOR_DEFAULT);
    level_add_point(g_level, vis_c_cyan);

    g_crystal_purple = light_new(63.0f, 18.0f, 11.0f, 1.25f, color_rgb(220, 50, 255));
    light_set_cast_shadows(g_crystal_purple, 1);
    level_add_light(g_level, g_crystal_purple);

    struct GameObject_Point *vis_c_purple = point_new(63, 18, '+');
    point_set_color(vis_c_purple, color_rgb(240, 140, 255), COLOR_DEFAULT);
    level_add_point(g_level, vis_c_purple);

    g_crystal_green = light_new(40.0f, 5.0f, 9.0f, 1.05f, color_rgb(40, 255, 110));
    light_set_cast_shadows(g_crystal_green, 1);
    level_add_light(g_level, g_crystal_green);

    struct GameObject_Point *vis_c_green = point_new(40, 5, '&');
    point_set_color(vis_c_green, color_rgb(100, 255, 160), COLOR_DEFAULT);
    level_add_point(g_level, vis_c_green);

    g_player_torch = light_new(g_hero_x, g_hero_y, 13.0f, 1.35f, color_rgb(255, 200, 95));
    light_set_flicker(g_player_torch, 0.10f, 8.0f);
    light_set_cast_shadows(g_player_torch, 1);
    level_add_light(g_level, g_player_torch);

    g_hero = player_new((int)g_hero_x, (int)g_hero_y, '@', 15.0f);
    player_set_color(g_hero, color_rgb(255, 255, 255), COLOR_DEFAULT);
    level_add_player(g_level, g_hero);

    level_set_hud_separator(g_level, HUD_TOP, '=', color_rgb(255, 190, 40), COLOR_DEFAULT);
    level_set_hud_text(g_level, HUD_TOP, 0, " === GLYPH ENGINE : DYNAMIC LIGHTING & HARD SHADOWS CASTING ===", color_rgb(255, 230, 140), COLOR_DEFAULT);
    level_set_hud_text(g_level, HUD_TOP, 1, " Torch [F]: ON   |  Shadows & Lights [L]: ON   |  Ambient [C]: DUNGEON DARK", color_rgb(255, 220, 120), COLOR_DEFAULT);

    level_set_hud_separator(g_level, HUD_BOTTOM, '=', color_rgb(0, 220, 255), COLOR_DEFAULT);
    level_set_hud_text(g_level, HUD_BOTTOM, 0, " [WASD / ZQSD / Arrows] Move  |  [F] Torch  |  [L] Toggle Shadows  |  [C] Ambient  |  [ESC] Exit", color_rgb(200, 240, 255), COLOR_DEFAULT);

    g_engine = engine_new(g_level, width, height);
    engine_set_exit_on_escape(g_engine, 1);
    engine_set_update_callback(g_engine, update);

    engine_run(g_engine, 60);

    engine_free(g_engine);
    return 0;
}
