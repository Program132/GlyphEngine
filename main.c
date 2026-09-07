#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/levelworld/levelworld.h"
#include "src/levelui/levelui.h"
#include "src/ui/button/button.h"
#include "src/ui/panel/panel.h"
#include "src/gameobject/player/player.h"
#include "src/gameobject/rectangle/rectangle.h"
#include "src/gameobject/point/point.h"
#include "src/gameobject/text/text.h"
#include "src/physics/physics.h"
#include "src/collision/collision.h"
#include "src/color/color.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

enum GameState {
    STATE_TITLE_MENU = 0,
    STATE_PLAYING,
    STATE_DYING,
    STATE_GAMEOVER,
    STATE_VICTORY
};

static struct Engine *g_engine = NULL;
static struct LevelUI *g_title_ui = NULL;
static struct LevelUI *g_gameover_ui = NULL;
static struct LevelUI *g_victory_ui = NULL;
static struct LevelWorld *g_world = NULL;
static struct GameObject_Player *g_hero = NULL;

static struct GameObject_Point *g_potions[6];
static int g_potions_active[6];

static struct GameObject_Point *g_spikes[10];

static struct GameObject_Rectangle *g_gate = NULL;
static struct GameObject_Point *g_pressure_plate = NULL;
static int g_gate_opened = 0;

static struct GameObject_Rectangle *g_loose_tiles[4];
static struct PhysicsBody g_loose_bodies[4];
static int g_loose_triggered[4];

static struct GameObject_Player *g_guard = NULL;
static int g_guard_alive = 1;
static int g_guard_hp = 3;
static float g_guard_attack_cooldown = 0.0f;

static float g_game_time = 0.0f;
static int g_has_sword = 0;
static struct GameObject_Point *g_sword_item = NULL;

static enum GameState g_state = STATE_TITLE_MENU;
static float g_death_timer = 0.0f;
static float g_invuln_timer = 0.0f;

static struct GameObject_Text *g_go_time_text = NULL;
static struct GameObject_Text *g_vic_time_text = NULL;

static int hero_touches_point(struct GameObject_Player *p, struct GameObject_Point *pt) {
    if (p == NULL || pt == NULL) return 0;
    float dx = fabsf(p->exact_x - (float)pt->position.x);
    float dy = fabsf(p->exact_y - (float)pt->position.y);
    return (dx <= 1.8f && dy <= 1.8f);
}

static void reset_dungeon(void) {
    g_game_time = 0.0f;
    g_gate_opened = 0;
    g_has_sword = 0;
    g_guard_alive = 1;
    g_guard_hp = 3;
    g_guard_attack_cooldown = 0.0f;
    g_death_timer = 0.0f;
    g_invuln_timer = 0.0f;

    struct Level *lvl = level_world_as_level(g_world);

    player_set_position(g_hero, 6, 36);
    g_hero->exact_x = 6.0f;
    g_hero->exact_y = 36.0f;
    g_hero->vx = 0.0f;
    g_hero->vy = 0.0f;
    g_hero->character = '@';
    player_set_health(g_hero, 3);
    player_set_speed(g_hero, 16.0f);
    player_set_color(g_hero, COLOR_BRIGHT_WHITE, COLOR_DEFAULT);

    g_world->base.cam_x = 0.0f;
    g_world->base.cam_y = 0.0f;

    if (g_gate != NULL) {
        g_gate->is_solid = 1;
        g_gate->character = '|';
        g_gate->fg = COLOR_BRIGHT_RED;
    }

    if (g_pressure_plate != NULL) {
        g_pressure_plate->character = '=';
        g_pressure_plate->fg = COLOR_BRIGHT_YELLOW;
    }

    if (g_sword_item != NULL) {
        g_sword_item->character = '/';
        g_sword_item->fg = COLOR_BRIGHT_CYAN;
    }

    for (int i = 0; i < 6; i++) {
        g_potions_active[i] = 1;
        if (g_potions[i] != NULL) {
            g_potions[i]->character = '!';
            g_potions[i]->fg = COLOR_BRIGHT_RED;
        }
    }

    int loose_x[4] = {55, 106, 125, 132};
    int loose_y[4] = {30, 32, 18, 18};
    for (int i = 0; i < 4; i++) {
        g_loose_triggered[i] = 0;
        if (g_loose_tiles[i] != NULL) {
            g_loose_tiles[i]->position.x = loose_x[i];
            g_loose_tiles[i]->position.y = loose_y[i];
            g_loose_tiles[i]->is_solid = 1;
            rectangle_set_color(g_loose_tiles[i], COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
        }
    }

    if (g_guard != NULL) {
        player_set_position(g_guard, 175, 16);
        g_guard->exact_x = 175.0f;
        g_guard->exact_y = 16.0f;
        g_guard->vx = 0.0f;
        g_guard->vy = 0.0f;
        player_set_health(g_guard, 3);
        g_guard->character = 'G';
        player_set_color(g_guard, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    }

    particle_system_clear(lvl->particle_system);
}

static void on_start_clicked(struct UIButton *btn, void *user_data) {
    (void)btn;
    (void)user_data;
    reset_dungeon();
    g_state = STATE_PLAYING;
    engine_set_level_ui(g_engine, NULL);
    engine_set_level(g_engine, level_world_as_level(g_world));
}

static void on_menu_clicked(struct UIButton *btn, void *user_data) {
    (void)btn;
    (void)user_data;
    g_state = STATE_TITLE_MENU;
    engine_set_level(g_engine, NULL);
    engine_set_level_ui(g_engine, g_title_ui);
}

static void on_quit_clicked(struct UIButton *btn, void *user_data) {
    (void)btn;
    (void)user_data;
    engine_stop(g_engine);
}

static void hero_take_hit(int dmg) {
    if (g_hero == NULL || g_state != STATE_PLAYING) return;
    if (g_invuln_timer > 0.0f) return;
    player_take_damage(g_hero, dmg);
    g_invuln_timer = 0.8f;
    struct Level *lvl = level_world_as_level(g_world);
    level_spawn_particles_sparkle(lvl, g_hero->exact_x, g_hero->exact_y, 8, COLOR_BRIGHT_RED);
    if (!player_is_alive(g_hero)) {
        g_state = STATE_DYING;
        g_death_timer = 1.6f;
        g_hero->vy = -8.0f;
        g_hero->character = 'X';
        player_set_color(g_hero, COLOR_BRIGHT_RED, COLOR_DEFAULT);
        level_spawn_particles_explosion(lvl, g_hero->exact_x, g_hero->exact_y, 25, COLOR_BRIGHT_RED);
    }
}

static void update(struct Engine *engine, float dt) {
    static int prev_esc = 0;
    int esc = input_is_key_down(KEY_ESCAPE);

    if (esc && !prev_esc) {
        if (g_state == STATE_PLAYING || g_state == STATE_DYING) {
            g_state = STATE_TITLE_MENU;
            engine_set_level(engine, NULL);
            engine_set_level_ui(engine, g_title_ui);
            prev_esc = esc;
            return;
        } else {
            engine_stop(engine);
            prev_esc = esc;
            return;
        }
    }
    prev_esc = esc;

    if (g_state == STATE_TITLE_MENU) {
        level_ui_handle_input(g_title_ui);
        return;
    }

    if (g_state == STATE_GAMEOVER) {
        level_ui_handle_input(g_gameover_ui);
        return;
    }

    if (g_state == STATE_VICTORY) {
        level_ui_handle_input(g_victory_ui);
        return;
    }

    if (g_state == STATE_DYING) {
        struct Level *lvl = level_world_as_level(g_world);
        g_death_timer -= dt;
        physics_simulate_player(g_hero, lvl, 0.0f, 0, dt);
        if (g_death_timer > 1.0f) {
            g_hero->character = 'X';
            player_set_color(g_hero, COLOR_BRIGHT_RED, COLOR_DEFAULT);
        } else if (g_death_timer > 0.5f) {
            g_hero->character = '%';
            player_set_color(g_hero, COLOR_RED, COLOR_DEFAULT);
            level_spawn_particles_sparkle(lvl, g_hero->exact_x, g_hero->exact_y, 2, COLOR_RED);
        } else {
            g_hero->character = '_';
            player_set_color(g_hero, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);
        }
        level_update(lvl, dt);
        if (g_death_timer <= 0.0f) {
            g_state = STATE_GAMEOVER;
            char buf[64];
            snprintf(buf, sizeof(buf), "Time survived: %02d:%02d", (int)g_game_time / 60, (int)g_game_time % 60);
            text_set_content(g_go_time_text, buf);
            engine_set_level(g_engine, NULL);
            engine_set_level_ui(g_engine, g_gameover_ui);
        }
        return;
    }

    struct Level *lvl = level_world_as_level(g_world);
    g_game_time += dt;
    if (g_invuln_timer > 0.0f) {
        g_invuln_timer -= dt;
        if (((int)(g_invuln_timer * 10.0f)) % 2 == 0) {
            player_set_color(g_hero, COLOR_BRIGHT_RED, COLOR_DEFAULT);
        } else {
            player_set_color(g_hero, COLOR_BRIGHT_WHITE, COLOR_DEFAULT);
        }
    } else {
        player_set_color(g_hero, COLOR_BRIGHT_WHITE, COLOR_DEFAULT);
    }

    float move_x = 0.0f;
    if (input_is_key_down(KEY_LEFT) || input_is_key_down('q') || input_is_key_down('a') || input_is_key_down('Q') || input_is_key_down('A')) move_x -= 1.0f;
    if (input_is_key_down(KEY_RIGHT) || input_is_key_down('d') || input_is_key_down('D')) move_x += 1.0f;

    int jump_pressed = (input_is_key_down(KEY_SPACE) || input_is_key_down(KEY_UP) || input_is_key_down('z') || input_is_key_down('w') || input_is_key_down('Z') || input_is_key_down('W'));

    physics_simulate_player(g_hero, lvl, move_x, jump_pressed, dt);

    if (g_hero->exact_y > 45.0f) {
        hero_take_hit(100);
        return;
    }

    level_world_follow_player(g_world, g_hero, 6.0f, dt);

    for (int i = 0; i < 4; i++) {
        if (g_loose_tiles[i] == NULL) continue;
        if (!g_loose_triggered[i]) {
            float hx = g_hero->exact_x;
            float hy = g_hero->exact_y;
            float tx = (float)g_loose_tiles[i]->position.x;
            float ty = (float)g_loose_tiles[i]->position.y;
            float tw = (float)g_loose_tiles[i]->width;
            if (hx + 1.0f > tx && hx < tx + tw && hy + 1.0f >= ty - 0.2f && hy <= ty + 1.0f) {
                g_loose_triggered[i] = 1;
                g_loose_tiles[i]->is_solid = 0;
                physics_body_init(&g_loose_bodies[i], tx, ty, 45.0f, 0.0f);
                level_spawn_particles_sparkle(lvl, tx + 2.0f, ty, 6, COLOR_YELLOW);
            }
        } else {
            physics_simulate_rectangle(g_loose_tiles[i], &g_loose_bodies[i], lvl, dt);
        }
    }

    if (g_pressure_plate != NULL && !g_gate_opened) {
        if (hero_touches_point(g_hero, g_pressure_plate)) {
            g_gate_opened = 1;
            g_pressure_plate->character = '_';
            g_pressure_plate->fg = COLOR_BRIGHT_GREEN;
            if (g_gate != NULL) {
                g_gate->is_solid = 0;
                g_gate->character = ':';
                g_gate->fg = COLOR_GREEN;
            }
            level_spawn_particles_sparkle(lvl, (float)g_pressure_plate->position.x, (float)g_pressure_plate->position.y, 15, COLOR_BRIGHT_GREEN);
        }
    }

    for (int i = 0; i < 10; i++) {
        if (g_spikes[i] != NULL && hero_touches_point(g_hero, g_spikes[i])) {
            hero_take_hit(1);
            g_hero->vy = -12.0f;
            break;
        }
    }

    for (int i = 0; i < 6; i++) {
        if (g_potions[i] != NULL && g_potions_active[i] && hero_touches_point(g_hero, g_potions[i])) {
            g_potions_active[i] = 0;
            g_potions[i]->character = ' ';
            player_heal(g_hero, 1);
            level_spawn_particles_sparkle(lvl, (float)g_potions[i]->position.x, (float)g_potions[i]->position.y, 15, COLOR_BRIGHT_RED);
        }
    }

    if (g_sword_item != NULL && !g_has_sword && hero_touches_point(g_hero, g_sword_item)) {
        g_has_sword = 1;
        g_sword_item->character = ' ';
        player_set_speed(g_hero, 20.0f);
        level_spawn_particles_sparkle(lvl, (float)g_sword_item->position.x, (float)g_sword_item->position.y, 25, COLOR_BRIGHT_CYAN);
    }

    if (g_guard != NULL && g_guard_alive) {
        float dist_x = g_guard->exact_x - g_hero->exact_x;
        float dist_y = g_guard->exact_y - g_hero->exact_y;
        if (dist_y > -4.0f && dist_y < 4.0f && dist_x > -25.0f && dist_x < 25.0f) {
            float guard_dir = (dist_x > 0.0f) ? -1.0f : 1.0f;
            physics_simulate_player(g_guard, lvl, guard_dir * 0.5f, 0, dt);
        } else {
            physics_simulate_player(g_guard, lvl, 0.0f, 0, dt);
        }

        g_guard_attack_cooldown -= dt;
        if (collision_check_player_player(g_hero, g_guard)) {
            if (input_is_key_down('e') || input_is_key_down('x') || input_is_key_down('E') || input_is_key_down('X')) {
                if (g_has_sword) {
                    g_guard_hp--;
                    level_spawn_particles_sparkle(lvl, g_guard->exact_x, g_guard->exact_y, 10, COLOR_BRIGHT_YELLOW);
                    if (g_guard_hp <= 0) {
                        g_guard_alive = 0;
                        g_guard->character = '%';
                        g_guard->fg = COLOR_RED;
                        level_spawn_particles_explosion(lvl, g_guard->exact_x, g_guard->exact_y, 20, COLOR_RED);
                    }
                } else {
                    hero_take_hit(1);
                }
            } else if (g_guard_attack_cooldown <= 0.0f) {
                hero_take_hit(1);
                g_guard_attack_cooldown = 1.2f;
                g_hero->vx = (dist_x > 0.0f) ? -10.0f : 10.0f;
            }
        }
    }

    if (g_hero->exact_x >= 188.0f && g_hero->exact_y >= 10.0f && g_hero->exact_y <= 16.0f) {
        g_state = STATE_VICTORY;
        char buf[64];
        snprintf(buf, sizeof(buf), "Clear Time: %02d:%02d", (int)g_game_time / 60, (int)g_game_time % 60);
        text_set_content(g_vic_time_text, buf);
        engine_set_level(engine, NULL);
        engine_set_level_ui(engine, g_victory_ui);
        return;
    }

    level_update(lvl, dt);

    char top_hud[256];
    char hp_pips[16] = "";
    for (int i = 0; i < g_hero->health && i < 10; i++) strcat(hp_pips, "[]");
    snprintf(top_hud, sizeof(top_hud), " HERO HP: %s (%d/%d) | SWORD: %s | TIME: %02d:%02d | LEVEL: THE ANCIENT DUNGEON ",
             hp_pips, g_hero->health, g_hero->max_health,
             g_has_sword ? "READY (/)" : "NONE",
             (int)g_game_time / 60, (int)g_game_time % 60);
    level_set_hud_text(lvl, HUD_TOP, 0, top_hud, COLOR_BRIGHT_YELLOW, COLOR_BLACK);

    char bottom_hud[256];
    snprintf(bottom_hud, sizeof(bottom_hud), " [A/D] Run | [W/SPACE] Jump | [E/X] Strike | Pos: (%d,%d) | Gate: %s | [ESC] Menu ",
             (int)g_hero->exact_x, (int)g_hero->exact_y,
             g_gate_opened ? "OPEN" : "LOCKED");
    level_set_hud_text(lvl, HUD_BOTTOM, 0, bottom_hud, COLOR_CYAN, COLOR_BLACK);
}

static void add_platform(struct LevelWorld *world, int x, int y, int w, int h, Color fg) {
    struct GameObject_Rectangle *rect = rectangle_new(x, y, w, h, '=');
    rectangle_set_color(rect, fg, COLOR_DEFAULT);
    rectangle_set_solid(rect, 1);
    level_add_rectangle(level_world_as_level(world), rect);
}

static void add_wall(struct LevelWorld *world, int x, int y, int w, int h, Color fg) {
    struct GameObject_Rectangle *rect = rectangle_new(x, y, w, h, '#');
    rectangle_set_color(rect, fg, COLOR_DEFAULT);
    rectangle_set_solid(rect, 1);
    level_add_rectangle(level_world_as_level(world), rect);
}

static void add_pillar(struct LevelWorld *world, int x, int y, int h) {
    struct GameObject_Rectangle *rect = rectangle_new(x, y, 2, h, '|');
    rectangle_set_color(rect, COLOR_YELLOW, COLOR_DEFAULT);
    rectangle_set_solid(rect, 0);
    level_add_rectangle(level_world_as_level(world), rect);
}

static void build_title_ui(void) {
    g_title_ui = level_ui_new("Title Menu", 90, 28, ' ');
    level_ui_set_default_color(g_title_ui, COLOR_WHITE, COLOR_DEFAULT);

    struct UIPanel *p_title = panel_new_styled(4, 1, 82, 5, " GLYPH ENGINE RETRO ADVENTURE ", '=', ' ', COLOR_BRIGHT_YELLOW, COLOR_BLACK);
    level_ui_add_panel(g_title_ui, p_title);

    struct GameObject_Text *t_banner = text_new_colored(16, 3, "D U N G E O N   E S C A P E   ( 1 9 8 9 )", COLOR_BRIGHT_CYAN, COLOR_BLACK);
    level_ui_add_text(g_title_ui, t_banner);

    struct UIPanel *p_story = panel_new_styled(4, 7, 52, 17, " STORY & OBJECTIVES ", '#', ' ', COLOR_YELLOW, COLOR_DEFAULT);
    level_ui_add_panel(g_title_ui, p_story);

    struct GameObject_Text *t_s1 = text_new_colored(6, 9, "Trapped deep within the ancient subterranean fortress!", COLOR_WHITE, COLOR_DEFAULT);
    struct GameObject_Text *t_s2 = text_new_colored(6, 10, "Escape the dungeon before the sands of time run out!", COLOR_BRIGHT_WHITE, COLOR_DEFAULT);
    struct GameObject_Text *t_s3 = text_new_colored(6, 12, "MISSION OBJECTIVES:", COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    struct GameObject_Text *t_s4 = text_new_colored(6, 13, "1. Run & jump over pits and dangerous spikes [^]", COLOR_WHITE, COLOR_DEFAULT);
    struct GameObject_Text *t_s5 = text_new_colored(6, 14, "2. Step on the Pressure Plate [=] to raise Gate [|]", COLOR_WHITE, COLOR_DEFAULT);
    struct GameObject_Text *t_s6 = text_new_colored(6, 15, "3. Find your trusty Sword [/] at (72, 14)", COLOR_WHITE, COLOR_DEFAULT);
    struct GameObject_Text *t_s7 = text_new_colored(6, 16, "4. Collect Red Potions [!] to restore lost HP", COLOR_WHITE, COLOR_DEFAULT);
    struct GameObject_Text *t_s8 = text_new_colored(6, 17, "5. Strike the Dungeon Guard [G] using [E] or [X]", COLOR_WHITE, COLOR_DEFAULT);
    struct GameObject_Text *t_s9 = text_new_colored(6, 18, "6. Reach the Palace Exit [D] at (190, 13)!", COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    struct GameObject_Text *t_s10 = text_new_colored(6, 20, "CONTROLS: [A/D] Move  |  [W/Space] Jump  |  [E/X] Attack", COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    struct GameObject_Text *t_s11 = text_new_colored(6, 22, "LEGEND: @:Hero  G:Guard  /:Sword  !:Potion  ^:Spike", COLOR_BRIGHT_MAGENTA, COLOR_DEFAULT);

    level_ui_add_text(g_title_ui, t_s1);
    level_ui_add_text(g_title_ui, t_s2);
    level_ui_add_text(g_title_ui, t_s3);
    level_ui_add_text(g_title_ui, t_s4);
    level_ui_add_text(g_title_ui, t_s5);
    level_ui_add_text(g_title_ui, t_s6);
    level_ui_add_text(g_title_ui, t_s7);
    level_ui_add_text(g_title_ui, t_s8);
    level_ui_add_text(g_title_ui, t_s9);
    level_ui_add_text(g_title_ui, t_s10);
    level_ui_add_text(g_title_ui, t_s11);

    struct UIPanel *p_actions = panel_new_styled(58, 7, 28, 17, " ACTIONS ", '#', ' ', COLOR_BRIGHT_BLUE, COLOR_DEFAULT);
    level_ui_add_panel(g_title_ui, p_actions);

    struct UIButton *btn_start = button_new(62, 10, 20, 3, "START ADVENTURE");
    button_set_on_click(btn_start, on_start_clicked, NULL);
    level_ui_add_button(g_title_ui, btn_start);

    struct UIButton *btn_quit = button_new(62, 16, 20, 3, "EXIT TO DESKTOP");
    button_set_on_click(btn_quit, on_quit_clicked, NULL);
    level_ui_add_button(g_title_ui, btn_quit);

    struct GameObject_Text *t_nav = text_new_colored(60, 22, "[UP/DOWN] Select  [ENTER] Confirm", COLOR_BRIGHT_BLACK, COLOR_DEFAULT);
    level_ui_add_text(g_title_ui, t_nav);
}

static void build_gameover_ui(void) {
    g_gameover_ui = level_ui_new("Game Over", 90, 28, ' ');
    level_ui_set_default_color(g_gameover_ui, COLOR_WHITE, COLOR_DEFAULT);

    struct UIPanel *box = panel_new_styled(20, 4, 50, 20, " GAME OVER ", '!', ' ', COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_ui_add_panel(g_gameover_ui, box);

    struct GameObject_Text *t_msg = text_new_colored(27, 7, "YOU HAVE PERISHED IN THE DUNGEON", COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_ui_add_text(g_gameover_ui, t_msg);

    g_go_time_text = text_new_colored(34, 10, "Time survived: 00:00", COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_ui_add_text(g_gameover_ui, g_go_time_text);

    struct UIButton *btn_retry = button_new(33, 13, 24, 3, "RETRY DUNGEON");
    button_set_on_click(btn_retry, on_start_clicked, NULL);
    level_ui_add_button(g_gameover_ui, btn_retry);

    struct UIButton *btn_menu = button_new(33, 17, 24, 3, "MAIN MENU");
    button_set_on_click(btn_menu, on_menu_clicked, NULL);
    level_ui_add_button(g_gameover_ui, btn_menu);
}

static void build_victory_ui(void) {
    g_victory_ui = level_ui_new("Victory", 90, 28, ' ');
    level_ui_set_default_color(g_victory_ui, COLOR_WHITE, COLOR_DEFAULT);

    struct UIPanel *box = panel_new_styled(18, 4, 54, 20, " ESCAPE SUCCESSFUL ", '*', ' ', COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
    level_ui_add_panel(g_victory_ui, box);

    struct GameObject_Text *t_congrats = text_new_colored(26, 7, "*** YOU HAVE ESCAPED THE DUNGEON! ***", COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_ui_add_text(g_victory_ui, t_congrats);

    struct GameObject_Text *t_desc = text_new_colored(23, 9, "You conquered all traps and broke free into the light!", COLOR_WHITE, COLOR_DEFAULT);
    level_ui_add_text(g_victory_ui, t_desc);

    g_vic_time_text = text_new_colored(35, 11, "Clear Time: 00:00", COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_ui_add_text(g_victory_ui, g_vic_time_text);

    struct UIButton *btn_again = button_new(33, 14, 24, 3, "PLAY AGAIN");
    button_set_on_click(btn_again, on_start_clicked, NULL);
    level_ui_add_button(g_victory_ui, btn_again);

    struct UIButton *btn_menu = button_new(33, 18, 24, 3, "MAIN MENU");
    button_set_on_click(btn_menu, on_menu_clicked, NULL);
    level_ui_add_button(g_victory_ui, btn_menu);
}

int main(void) {
    g_world = level_world_new("Dungeon Escape (1989)", 200, 50, 90, 28, ' ');
    struct Level *lvl = level_world_as_level(g_world);
    level_set_default_color(lvl, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);

    add_wall(g_world, 0, 0, 200, 2, COLOR_YELLOW);
    add_wall(g_world, 0, 48, 200, 2, COLOR_YELLOW);
    add_wall(g_world, 0, 0, 2, 50, COLOR_YELLOW);
    add_wall(g_world, 198, 0, 2, 50, COLOR_YELLOW);

    add_platform(g_world, 2, 38, 45, 2, COLOR_YELLOW);
    add_platform(g_world, 30, 35, 8, 2, COLOR_YELLOW);
    add_platform(g_world, 36, 32, 8, 2, COLOR_YELLOW);
    add_platform(g_world, 40, 30, 28, 2, COLOR_YELLOW);
    add_platform(g_world, 15, 22, 28, 2, COLOR_YELLOW);
    add_platform(g_world, 50, 15, 30, 2, COLOR_YELLOW);

    add_pillar(g_world, 10, 24, 14);
    add_pillar(g_world, 35, 24, 14);
    add_pillar(g_world, 48, 17, 13);
    add_pillar(g_world, 65, 17, 13);

    add_platform(g_world, 80, 42, 60, 2, COLOR_YELLOW);
    add_platform(g_world, 85, 32, 20, 2, COLOR_YELLOW);
    add_platform(g_world, 115, 25, 25, 2, COLOR_YELLOW);

    g_loose_tiles[0] = rectangle_new(55, 30, 4, 1, '~');
    rectangle_set_color(g_loose_tiles[0], COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    rectangle_set_solid(g_loose_tiles[0], 1);
    level_add_rectangle(lvl, g_loose_tiles[0]);

    g_loose_tiles[1] = rectangle_new(106, 32, 4, 1, '~');
    rectangle_set_color(g_loose_tiles[1], COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    rectangle_set_solid(g_loose_tiles[1], 1);
    level_add_rectangle(lvl, g_loose_tiles[1]);

    g_loose_tiles[2] = rectangle_new(125, 18, 4, 1, '~');
    rectangle_set_color(g_loose_tiles[2], COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    rectangle_set_solid(g_loose_tiles[2], 1);
    level_add_rectangle(lvl, g_loose_tiles[2]);

    g_loose_tiles[3] = rectangle_new(132, 18, 4, 1, '~');
    rectangle_set_color(g_loose_tiles[3], COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    rectangle_set_solid(g_loose_tiles[3], 1);
    level_add_rectangle(lvl, g_loose_tiles[3]);

    add_wall(g_world, 145, 15, 3, 33, COLOR_YELLOW);

    g_gate = rectangle_new(145, 36, 3, 6, '|');
    rectangle_set_color(g_gate, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    rectangle_set_solid(g_gate, 1);
    level_add_rectangle(lvl, g_gate);

    struct GameObject_Text *lbl_gate = text_new_colored(143, 34, "GATE [|]", COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_add_text(lvl, lbl_gate);

    g_pressure_plate = point_new(20, 21, '=');
    point_set_color(g_pressure_plate, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_point(lvl, g_pressure_plate);

    struct GameObject_Text *lbl_plate = text_new_colored(12, 20, "PLATE -->", COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_text(lvl, lbl_plate);

    add_platform(g_world, 148, 42, 45, 2, COLOR_YELLOW);
    add_platform(g_world, 155, 30, 40, 2, COLOR_YELLOW);
    add_platform(g_world, 150, 18, 45, 2, COLOR_YELLOW);

    struct GameObject_Rectangle *door = rectangle_new(190, 13, 5, 5, 'D');
    rectangle_set_color(door, COLOR_BRIGHT_YELLOW, COLOR_YELLOW);
    rectangle_set_solid(door, 0);
    level_add_rectangle(lvl, door);

    struct GameObject_Text *door_lbl = text_new_colored(189, 11, "EXIT [D]", COLOR_BRIGHT_WHITE, COLOR_DEFAULT);
    level_add_text(lvl, door_lbl);

    int spike_coords[10][2] = {
        {25, 37}, {26, 37}, {27, 37},
        {92, 41}, {93, 41}, {94, 41},
        {162, 41}, {163, 41}, {164, 41}, {165, 41}
    };
    for (int i = 0; i < 10; i++) {
        g_spikes[i] = point_new(spike_coords[i][0], spike_coords[i][1], '^');
        point_set_color(g_spikes[i], COLOR_BRIGHT_RED, COLOR_DEFAULT);
        level_add_point(lvl, g_spikes[i]);
    }

    int potion_coords[6][2] = {
        {5, 37}, {42, 29}, {70, 14}, {86, 31}, {130, 24}, {170, 29}
    };
    for (int i = 0; i < 6; i++) {
        g_potions[i] = point_new(potion_coords[i][0], potion_coords[i][1], '!');
        point_set_color(g_potions[i], COLOR_BRIGHT_RED, COLOR_DEFAULT);
        g_potions_active[i] = 1;
        level_add_point(lvl, g_potions[i]);
    }

    g_sword_item = point_new(72, 14, '/');
    point_set_color(g_sword_item, COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_add_point(lvl, g_sword_item);

    struct GameObject_Text *lbl_sword = text_new_colored(62, 13, "SWORD [/] -->", COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_add_text(lvl, lbl_sword);

    g_hero = player_new(6, 36, '@', 16.0f);
    player_set_color(g_hero, COLOR_BRIGHT_WHITE, COLOR_DEFAULT);
    player_set_health(g_hero, 3);
    player_enable_gravity(g_hero, 40.0f);
    player_set_jump_power(g_hero, 22.0f);
    level_add_player(lvl, g_hero);

    struct GameObject_Text *lbl_start = text_new_colored(4, 34, "HERO [@]", COLOR_BRIGHT_WHITE, COLOR_DEFAULT);
    level_add_text(lvl, lbl_start);

    g_guard = player_new(175, 16, 'G', 8.0f);
    player_set_color(g_guard, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    player_set_health(g_guard, 3);
    player_enable_gravity(g_guard, 40.0f);
    level_add_player(lvl, g_guard);

    struct GameObject_Text *lbl_guard = text_new_colored(172, 14, "GUARD [G]", COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_add_text(lvl, lbl_guard);

    int torches[5][2] = {{12, 35}, {45, 27}, {95, 29}, {120, 22}, {180, 15}};
    for (int i = 0; i < 5; i++) {
        struct GameObject_Point *t = point_new(torches[i][0], torches[i][1], '*');
        point_set_color(t, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
        level_add_point(lvl, t);
    }

    level_set_hud_separator(lvl, HUD_TOP, '=', COLOR_YELLOW, COLOR_DEFAULT);
    level_set_hud_separator(lvl, HUD_BOTTOM, '=', COLOR_YELLOW, COLOR_DEFAULT);

    build_title_ui();
    build_gameover_ui();
    build_victory_ui();

    g_engine = engine_new(NULL, 90, 28);
    engine_set_exit_on_escape(g_engine, 0);
    engine_set_level_ui(g_engine, g_title_ui);
    engine_set_update_callback(g_engine, update);

    engine_run(g_engine, 60);

    engine_free(g_engine);
    return 0;
}