#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/level/level.h"
#include "src/gameobject/player/player.h"
#include "src/gameobject/rectangle/rectangle.h"
#include "src/gameobject/point/point.h"
#include "src/gameobject/text/text.h"
#include "src/physics/physics.h"
#include "src/color/color.h"
#include "src/particle/particle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static struct Engine *g_engine = NULL;
static struct Level *g_level = NULL;
static struct GameObject_Player *g_hero = NULL;
static float g_time = 0.0f;
static int g_prev_jump = 1;

#define FIRE_WIDTH 20
#define FIRE_HEIGHT 10
static struct GameObject_Point *g_fire_points[FIRE_HEIGHT][FIRE_WIDTH];

#define WATER_WIDTH 30
static struct GameObject_Point *g_water_points[WATER_WIDTH];

#define SPECTRUM_WIDTH 50
static struct GameObject_Point *g_spectrum_points[SPECTRUM_WIDTH];

static void update(struct Engine *engine, float dt) {
    if (dt > 0.04f) dt = 0.04f;
    g_time += dt;

    if (input_is_key_down(KEY_ESCAPE)) {
        engine_stop(engine);
        return;
    }

    float move_x = 0.0f;
    if (input_is_key_down(KEY_LEFT) || input_is_key_down('q') || input_is_key_down('a') || input_is_key_down('Q') || input_is_key_down('A')) move_x -= 1.0f;
    if (input_is_key_down(KEY_RIGHT) || input_is_key_down('d') || input_is_key_down('D')) move_x += 1.0f;

    int jump_raw = (input_is_key_down(KEY_SPACE) || input_is_key_down(KEY_UP) || input_is_key_down('z') || input_is_key_down('w') || input_is_key_down('Z') || input_is_key_down('W'));
    int jump_pressed = (jump_raw && !g_prev_jump);
    g_prev_jump = jump_raw;

    physics_simulate_player(g_hero, g_level, move_x, jump_pressed, dt);

    float hero_hue = fmodf(g_time * 2.0f, 6.28318f);
    unsigned char hr = (unsigned char)(127.0f + 127.0f * sinf(hero_hue));
    unsigned char hg = (unsigned char)(127.0f + 127.0f * sinf(hero_hue + 2.094f));
    unsigned char hb = (unsigned char)(127.0f + 127.0f * sinf(hero_hue + 4.188f));
    player_set_color(g_hero, color_rgb(hr, hg, hb), COLOR_DEFAULT);

    if (fabsf(move_x) > 0.1f && g_hero->is_grounded) {
        level_spawn_particles_sparkle(g_level, g_hero->exact_x, g_hero->exact_y, 1, color_rgb(hr, hg, hb));
    }

    Color cold_crimson = color_rgb(120, 0, 10);
    Color warm_orange = color_rgb(255, 100, 0);
    Color bright_gold = color_rgb(255, 220, 40);
    Color white_hot = color_rgb(255, 255, 240);

    for (int y = 0; y < FIRE_HEIGHT; y++) {
        float y_ratio = (float)y / (float)(FIRE_HEIGHT - 1);
        for (int x = 0; x < FIRE_WIDTH; x++) {
            float wave = sinf(g_time * 6.0f + (float)x * 0.6f + (float)y * 0.8f);
            float noise = ((float)(rand() % 100) / 100.0f) * 0.25f;
            float t = y_ratio + noise * 0.3f;
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            Color c;
            if (t < 0.4f) {
                c = color_lerp(white_hot, bright_gold, t / 0.4f);
            } else if (t < 0.75f) {
                c = color_lerp(bright_gold, warm_orange, (t - 0.4f) / 0.35f);
            } else {
                c = color_lerp(warm_orange, cold_crimson, (t - 0.75f) / 0.25f);
            }

            char ch = (t < 0.3f) ? '#' : ((t < 0.6f) ? '*' : ((t < 0.85f) ? '^' : '.'));
            if (wave > 0.85f && y > 7) ch = ' ';

            point_set_character(g_fire_points[y][x], ch);
            point_set_color(g_fire_points[y][x], c, COLOR_DEFAULT);
        }
    }

    Color deep_navy = color_rgb(10, 20, 80);
    Color electric_blue = color_rgb(0, 120, 255);
    Color turquoise = color_rgb(0, 230, 210);
    Color cyan_foam = color_rgb(200, 255, 255);

    for (int x = 0; x < WATER_WIDTH; x++) {
        float wave = sinf(g_time * 4.0f + (float)x * 0.35f);
        float t = (wave + 1.0f) * 0.5f;

        Color c;
        if (t < 0.5f) {
            c = color_lerp(deep_navy, electric_blue, t * 2.0f);
        } else if (t < 0.85f) {
            c = color_lerp(electric_blue, turquoise, (t - 0.5f) / 0.35f);
        } else {
            c = color_lerp(turquoise, cyan_foam, (t - 0.85f) / 0.15f);
        }

        char ch = (t > 0.8f) ? '~' : ((t > 0.4f) ? '=' : '-');
        point_set_character(g_water_points[x], ch);
        point_set_color(g_water_points[x], c, COLOR_DEFAULT);
    }

    for (int x = 0; x < SPECTRUM_WIDTH; x++) {
        float factor = (float)x / (float)(SPECTRUM_WIDTH - 1);
        float hue = factor * 6.28318f + g_time * 1.5f;
        unsigned char r = (unsigned char)(127.0f + 127.0f * sinf(hue));
        unsigned char g = (unsigned char)(127.0f + 127.0f * sinf(hue + 2.094f));
        unsigned char b = (unsigned char)(127.0f + 127.0f * sinf(hue + 4.188f));
        point_set_color(g_spectrum_points[x], color_rgb(r, g, b), COLOR_DEFAULT);
    }

    float fountain_x = 55.0f;
    float fountain_y = 12.0f;
    for (int p = 0; p < 2; p++) {
        float f_hue = fmodf(g_time * 5.0f + (float)p, 6.28318f);
        unsigned char pr = (unsigned char)(127.0f + 127.0f * sinf(f_hue));
        unsigned char pg = (unsigned char)(127.0f + 127.0f * sinf(f_hue + 2.094f));
        unsigned char pb = (unsigned char)(127.0f + 127.0f * sinf(f_hue + 4.188f));
        level_spawn_particles_sparkle(g_level, fountain_x, fountain_y, 1, color_rgb(pr, pg, pb));
    }

    level_update(g_level, dt);

    char top_buf[256];
    snprintf(top_buf, sizeof(top_buf), " GLYPH ENGINE 24-BIT TRUECOLOR SHOWCASE | Pos: (%d,%d) | Time: %.1fs ",
             (int)g_hero->exact_x, (int)g_hero->exact_y, g_time);
    level_set_hud_text(g_level, HUD_TOP, 0, top_buf, color_rgb(255, 215, 0), color_rgb(20, 20, 30));

    char bot_buf[256];
    snprintf(bot_buf, sizeof(bot_buf), " [A/D] Move | [W/Space] Jump | RGB Fire, Plasma & Dynamic Lighting | [ESC] Quit ");
    level_set_hud_text(g_level, HUD_BOTTOM, 0, bot_buf, color_rgb(0, 230, 255), color_rgb(15, 20, 25));
}

int main(void) {
    g_level = level_new("TrueColor Showcase", 90, 28, ' ');
    level_set_default_color(g_level, color_rgb(60, 60, 75), COLOR_DEFAULT);

    struct GameObject_Rectangle *top_wall = rectangle_new(0, 0, 90, 1, '#');
    rectangle_set_color(top_wall, color_rgb(100, 100, 130), color_rgb(15, 15, 25));
    rectangle_set_solid(top_wall, 1);
    level_add_rectangle(g_level, top_wall);

    struct GameObject_Rectangle *bottom_wall = rectangle_new(0, 27, 90, 1, '#');
    rectangle_set_color(bottom_wall, color_rgb(100, 100, 130), color_rgb(15, 15, 25));
    rectangle_set_solid(bottom_wall, 1);
    level_add_rectangle(g_level, bottom_wall);

    struct GameObject_Rectangle *left_wall = rectangle_new(0, 0, 1, 28, '#');
    rectangle_set_color(left_wall, color_rgb(100, 100, 130), color_rgb(15, 15, 25));
    rectangle_set_solid(left_wall, 1);
    level_add_rectangle(g_level, left_wall);

    struct GameObject_Rectangle *right_wall = rectangle_new(89, 0, 1, 28, '#');
    rectangle_set_color(right_wall, color_rgb(100, 100, 130), color_rgb(15, 15, 25));
    rectangle_set_solid(right_wall, 1);
    level_add_rectangle(g_level, right_wall);

    struct GameObject_Rectangle *ground = rectangle_new(1, 25, 88, 2, '=');
    rectangle_set_color(ground, color_rgb(90, 70, 50), COLOR_DEFAULT);
    rectangle_set_solid(ground, 1);
    level_add_rectangle(g_level, ground);

    struct GameObject_Rectangle *plat1 = rectangle_new(5, 19, 18, 1, '=');
    rectangle_set_color(plat1, color_rgb(160, 100, 45), COLOR_DEFAULT);
    rectangle_set_solid(plat1, 1);
    level_add_rectangle(g_level, plat1);

    struct GameObject_Rectangle *plat2 = rectangle_new(48, 13, 15, 1, '=');
    rectangle_set_color(plat2, color_rgb(180, 120, 60), COLOR_DEFAULT);
    rectangle_set_solid(plat2, 1);
    level_add_rectangle(g_level, plat2);

    struct GameObject_Rectangle *plat3 = rectangle_new(68, 18, 16, 1, '=');
    rectangle_set_color(plat3, color_rgb(160, 100, 45), COLOR_DEFAULT);
    rectangle_set_solid(plat3, 1);
    level_add_rectangle(g_level, plat3);

    struct GameObject_Text *lbl_fire = text_new_colored(33, 13, "RGB INFERNO", color_rgb(255, 140, 0), COLOR_DEFAULT);
    level_add_text(g_level, lbl_fire);

    for (int y = 0; y < FIRE_HEIGHT; y++) {
        for (int x = 0; x < FIRE_WIDTH; x++) {
            g_fire_points[y][x] = point_new(28 + x, 15 + y, '*');
            point_set_color(g_fire_points[y][x], color_rgb(255, 100, 0), COLOR_DEFAULT);
            level_add_point(g_level, g_fire_points[y][x]);
        }
    }

    struct GameObject_Text *lbl_water = text_new_colored(8, 22, "TRUECOLOR PLASMA WAVE", color_rgb(0, 200, 255), COLOR_DEFAULT);
    level_add_text(g_level, lbl_water);

    for (int x = 0; x < WATER_WIDTH; x++) {
        g_water_points[x] = point_new(5 + x, 24, '~');
        point_set_color(g_water_points[x], color_rgb(0, 150, 255), COLOR_DEFAULT);
        level_add_point(g_level, g_water_points[x]);
    }

    struct GameObject_Text *lbl_spectrum = text_new_colored(20, 2, "24-BIT CONTINUOUS COLOR SPECTRUM", color_rgb(255, 255, 255), COLOR_DEFAULT);
    level_add_text(g_level, lbl_spectrum);

    for (int x = 0; x < SPECTRUM_WIDTH; x++) {
        g_spectrum_points[x] = point_new(20 + x, 4, '#');
        point_set_color(g_spectrum_points[x], color_rgb(255, 255, 255), COLOR_DEFAULT);
        level_add_point(g_level, g_spectrum_points[x]);
    }

    struct GameObject_Text *lbl_ansi = text_new_colored(3, 7, "ANSI 16 CLASSIC: ", COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    level_add_text(g_level, lbl_ansi);

    struct GameObject_Point *ansi_pts[6];
    Color ansi_colors[6] = {COLOR_RED, COLOR_YELLOW, COLOR_GREEN, COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA};
    char ansi_chars[6] = {'R', 'Y', 'G', 'C', 'B', 'M'};
    for (int i = 0; i < 6; i++) {
        ansi_pts[i] = point_new(20 + i * 3, 7, ansi_chars[i]);
        point_set_color(ansi_pts[i], ansi_colors[i], COLOR_DEFAULT);
        level_add_point(g_level, ansi_pts[i]);
    }

    struct GameObject_Text *lbl_rgb = text_new_colored(42, 7, "TRUECOLOR RGB: ", color_rgb(255, 180, 50), COLOR_DEFAULT);
    level_add_text(g_level, lbl_rgb);

    struct GameObject_Point *rgb_pts[6];
    Color rgb_colors[6] = {COLOR_RGB_CRIMSON, COLOR_RGB_ORANGE, COLOR_RGB_GOLD, COLOR_RGB_LIME, COLOR_RGB_TEAL, COLOR_RGB_PURPLE};
    for (int i = 0; i < 6; i++) {
        rgb_pts[i] = point_new(58 + i * 3, 7, '@');
        point_set_color(rgb_pts[i], rgb_colors[i], COLOR_DEFAULT);
        level_add_point(g_level, rgb_pts[i]);
    }

    struct GameObject_Text *lbl_fountain = text_new_colored(48, 11, "RGB FOUNTAIN", color_rgb(255, 100, 255), COLOR_DEFAULT);
    level_add_text(g_level, lbl_fountain);

    g_hero = player_new(8, 17, '@', 14.0f);
    player_set_color(g_hero, color_rgb(255, 255, 255), COLOR_DEFAULT);
    player_enable_gravity(g_hero, 38.0f);
    player_set_jump_power(g_hero, 19.0f);
    level_add_player(g_level, g_hero);

    level_set_hud_separator(g_level, HUD_TOP, '=', color_rgb(255, 215, 0), COLOR_DEFAULT);
    level_set_hud_separator(g_level, HUD_BOTTOM, '=', color_rgb(0, 230, 255), COLOR_DEFAULT);

    g_engine = engine_new(g_level, 90, 28);
    engine_set_exit_on_escape(g_engine, 1);
    engine_set_update_callback(g_engine, update);

    engine_run(g_engine, 60);

    engine_free(g_engine);
    return 0;
}
