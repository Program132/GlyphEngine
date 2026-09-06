#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/levelui/levelui.h"
#include "src/ui/button/button.h"
#include "src/ui/panel/panel.h"
#include "src/gameobject/text/text.h"
#include "src/gameobject/player/player.h"
#include "src/gameobject/rectangle/rectangle.h"
#include "src/gameobject/circle/circle.h"
#include "src/projectile/projectile.h"
#include "src/particle/particle.h"
#include "src/collision/collision.h"
#include "src/color/color.h"
#include <stdio.h>
#include <stdlib.h>

enum GameState {
    STATE_MENU = 0,
    STATE_GAME,
    STATE_GAMEOVER
};

struct Engine *g_engine = NULL;
struct LevelUI *g_menu = NULL;
struct LevelUI *g_gameover_ui = NULL;
struct Level *g_game = NULL;
struct GameObject_Player *g_player = NULL;
struct GameObject_Rectangle *g_invader1 = NULL;
struct GameObject_Rectangle *g_invader2 = NULL;
struct GameObject_Ellipse *g_mothership = NULL;
struct GameObject_Text *g_final_score = NULL;

enum GameState g_state = STATE_MENU;
int g_score = 0;
int invader1_alive = 1;
int invader2_alive = 1;
int mothership_hp = 30;
float invader1_x = 10.0f, invader1_dx = 12.0f;
float invader2_x = 35.0f, invader2_dx = -15.0f;
float mothership_x = 25.0f, mothership_dx = 8.0f;
float enemy_shoot_timer = 0.0f;

void reset_game(void) {
    g_score = 0;
    player_set_health(g_player, 100);
    player_set_position(g_player, 30, 12);
    g_player->exact_x = 30.0f;
    g_player->exact_y = 12.0f;
    player_set_color(g_player, COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    
    invader1_alive = 1;
    invader2_alive = 1;
    mothership_hp = 30;
    invader1_x = 10.0f; invader1_dx = 12.0f;
    invader2_x = 35.0f; invader2_dx = -15.0f;
    mothership_x = 25.0f; mothership_dx = 8.0f;
    enemy_shoot_timer = 0.0f;

    for (int i = 0; i < MAX_LEVEL_PROJECTILES; i++) {
        if (g_game->projectiles[i] != NULL) {
            g_game->projectiles[i]->is_alive = 0;
        }
    }
    if (g_game->particle_system != NULL) {
        particle_system_clear(g_game->particle_system);
    }
}

void on_play_clicked(struct UIButton *btn, void *user_data) {
    (void)btn;
    (void)user_data;
    reset_game();
    g_state = STATE_GAME;
    engine_set_level_ui(g_engine, NULL);
    engine_set_level(g_engine, g_game);
}

void on_retry_clicked(struct UIButton *btn, void *user_data) {
    (void)btn;
    (void)user_data;
    reset_game();
    g_state = STATE_GAME;
    engine_set_level_ui(g_engine, NULL);
    engine_set_level(g_engine, g_game);
}

void on_menu_clicked(struct UIButton *btn, void *user_data) {
    (void)btn;
    (void)user_data;
    g_state = STATE_MENU;
    engine_set_level(g_engine, NULL);
    engine_set_level_ui(g_engine, g_menu);
}

void on_quit_clicked(struct UIButton *btn, void *user_data) {
    (void)btn;
    (void)user_data;
    engine_stop(g_engine);
}

void update(struct Engine *engine, float dt) {
    static int prev_esc = 0;
    static int prev_space = 0;
    int esc = input_is_key_down(KEY_ESCAPE);
    int space = input_is_key_down(KEY_SPACE);

    if (esc && !prev_esc) {
        if (g_state == STATE_GAME) {
            g_state = STATE_MENU;
            engine_set_level(engine, NULL);
            engine_set_level_ui(engine, g_menu);
            prev_esc = esc;
            return;
        } else {
            engine_stop(engine);
            prev_esc = esc;
            return;
        }
    }
    prev_esc = esc;

    if (g_state == STATE_MENU) {
        level_ui_handle_input(g_menu);
        return;
    }

    if (g_state == STATE_GAMEOVER) {
        level_ui_handle_input(g_gameover_ui);
        return;
    }

    float dx = 0.0f;
    float dy = 0.0f;

    if (input_is_key_down(KEY_UP) || input_is_key_down('z') || input_is_key_down('w')) dy -= 1.0f;
    if (input_is_key_down(KEY_DOWN) || input_is_key_down('s')) dy += 1.0f;
    if (input_is_key_down(KEY_LEFT) || input_is_key_down('q') || input_is_key_down('a')) dx -= 1.0f;
    if (input_is_key_down(KEY_RIGHT) || input_is_key_down('d')) dx += 1.0f;

    player_move(g_player, dx, dy, dt);
    collision_clamp_player(g_game, g_player);

    if (space && !prev_space) {
        level_spawn_projectile(g_game, g_player->exact_x, g_player->exact_y - 1.0f, 0.0f, -24.0f, '|', COLOR_BRIGHT_YELLOW, 10, 1.5f, PROJECTILE_PLAYER);
        level_spawn_particles_sparkle(g_game, g_player->exact_x, g_player->exact_y, 4, COLOR_YELLOW);
    }
    prev_space = space;

    if (invader1_alive) {
        invader1_x += invader1_dx * dt;
        if (invader1_x <= 2.0f || invader1_x >= g_game->sizeX - 10.0f) invader1_dx = -invader1_dx;
        g_invader1->position.x = (int)invader1_x;
    } else {
        g_invader1->position.x = -100;
    }

    if (invader2_alive) {
        invader2_x += invader2_dx * dt;
        if (invader2_x <= 2.0f || invader2_x >= g_game->sizeX - 10.0f) invader2_dx = -invader2_dx;
        g_invader2->position.x = (int)invader2_x;
    } else {
        g_invader2->position.x = -100;
    }

    if (mothership_hp > 0) {
        mothership_x += mothership_dx * dt;
        if (mothership_x <= 6.0f || mothership_x >= g_game->sizeX - 8.0f) mothership_dx = -mothership_dx;
        g_mothership->position.x = (int)mothership_x;
    } else {
        g_mothership->position.x = -100;
    }

    enemy_shoot_timer += dt;
    if (enemy_shoot_timer >= 1.2f) {
        enemy_shoot_timer = 0.0f;
        if (mothership_hp > 0) {
            level_spawn_projectile(g_game, mothership_x, 3.0f, 0.0f, 10.0f, 'v', COLOR_BRIGHT_RED, 15, 2.5f, PROJECTILE_ENEMY);
        }
        if (invader1_alive && (rand() % 2 == 0)) {
            level_spawn_projectile(g_game, invader1_x + 3.0f, 6.0f, 0.0f, 12.0f, '*', COLOR_BRIGHT_MAGENTA, 10, 2.0f, PROJECTILE_ENEMY);
        }
        if (invader2_alive && (rand() % 2 == 0)) {
            level_spawn_projectile(g_game, invader2_x + 3.0f, 6.0f, 0.0f, 12.0f, '*', COLOR_BRIGHT_MAGENTA, 10, 2.0f, PROJECTILE_ENEMY);
        }
    }

    level_update(g_game, dt);

    for (int i = 0; i < MAX_LEVEL_PROJECTILES; i++) {
        struct GameObject_Projectile *p = g_game->projectiles[i];
        if (p == NULL || !p->is_alive) continue;

        if (p->owner == PROJECTILE_PLAYER) {
            if (invader1_alive && collision_check_projectile_rect(p, g_invader1)) {
                p->is_alive = 0;
                invader1_alive = 0;
                g_score += 150;
                level_spawn_particles_explosion(g_game, p->exact_x, p->exact_y, 16, COLOR_BRIGHT_RED);
            } else if (invader2_alive && collision_check_projectile_rect(p, g_invader2)) {
                p->is_alive = 0;
                invader2_alive = 0;
                g_score += 150;
                level_spawn_particles_explosion(g_game, p->exact_x, p->exact_y, 16, COLOR_BRIGHT_RED);
            } else if (mothership_hp > 0 && collision_check_projectile_ellipse(p, g_mothership)) {
                p->is_alive = 0;
                mothership_hp -= p->damage;
                g_score += 50;
                level_spawn_particles_sparkle(g_game, p->exact_x, p->exact_y, 8, COLOR_BRIGHT_YELLOW);
                if (mothership_hp <= 0) {
                    g_score += 500;
                    level_spawn_particles_explosion(g_game, mothership_x, 2.0f, 25, COLOR_BRIGHT_YELLOW);
                }
            }
        } else if (p->owner == PROJECTILE_ENEMY) {
            if (collision_check_projectile_player(p, g_player)) {
                p->is_alive = 0;
                player_take_damage(g_player, p->damage);
                player_set_color(g_player, COLOR_BRIGHT_RED, COLOR_DEFAULT);
                level_spawn_particles_sparkle(g_game, g_player->exact_x, g_player->exact_y, 10, COLOR_RED);
            }
        }
    }

    if (invader1_alive == 0 && invader2_alive == 0 && mothership_hp <= 0) {
        invader1_alive = 1;
        invader2_alive = 1;
        mothership_hp = 40;
    }

    int hp = player_get_health(g_player);

    if (hp <= 0) {
        g_state = STATE_GAMEOVER;
        char score_buf[64];
        snprintf(score_buf, sizeof(score_buf), "Your Final Score: %d", g_score);
        text_set_content(g_final_score, score_buf);
        engine_set_level(engine, NULL);
        engine_set_level_ui(engine, g_gameover_ui);
        return;
    }

    char top_buf[80];
    snprintf(top_buf, sizeof(top_buf), " HP: %3d/100  |  SCORE: %5d  |  BOSS HP: %2d  |  [SPACE] Shoot ", hp, g_score, mothership_hp > 0 ? mothership_hp : 0);
    Color hud_color = (hp > 50) ? COLOR_BRIGHT_GREEN : (hp > 20 ? COLOR_BRIGHT_YELLOW : COLOR_BRIGHT_RED);
    level_set_hud_text(g_game, HUD_TOP, 0, top_buf, hud_color, COLOR_BLACK);
}

int main() {
    g_menu = level_ui_new("Main Menu", 60, 20, ' ');
    level_ui_set_default_color(g_menu, COLOR_WHITE, COLOR_DEFAULT);

    struct UIPanel *box = panel_new_styled(14, 2, 32, 16, " GLYPH ENGINE ", '#', ' ', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_ui_add_panel(g_menu, box);

    struct GameObject_Text *sub = text_new_colored(18, 4, "Projectiles & Particles", COLOR_YELLOW, COLOR_DEFAULT);
    level_ui_add_text(g_menu, sub);

    struct UIButton *btn_play = button_new(20, 7, 20, 3, "START GAME");
    button_set_on_click(btn_play, on_play_clicked, NULL);
    level_ui_add_button(g_menu, btn_play);

    struct UIButton *btn_quit = button_new(20, 12, 20, 3, "EXIT");
    button_set_on_click(btn_quit, on_quit_clicked, NULL);
    level_ui_add_button(g_menu, btn_quit);

    struct GameObject_Text *help = text_new_colored(10, 19, "UP/DOWN or TAB to focus, ENTER to click", COLOR_BRIGHT_BLACK, COLOR_DEFAULT);
    level_ui_add_text(g_menu, help);

    g_gameover_ui = level_ui_new("Game Over", 60, 20, ' ');
    level_ui_set_default_color(g_gameover_ui, COLOR_WHITE, COLOR_DEFAULT);

    struct UIPanel *go_box = panel_new_styled(12, 2, 36, 16, " GAME OVER ", '!', ' ', COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_ui_add_panel(g_gameover_ui, go_box);

    struct GameObject_Text *go_title = text_new_colored(23, 4, "YOU DIED!", COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_ui_add_text(g_gameover_ui, go_title);

    g_final_score = text_new_colored(18, 6, "Your Final Score: 0", COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_ui_add_text(g_gameover_ui, g_final_score);

    struct UIButton *btn_retry = button_new(18, 8, 24, 3, "RETRY");
    button_set_on_click(btn_retry, on_retry_clicked, NULL);
    level_ui_add_button(g_gameover_ui, btn_retry);

    struct UIButton *btn_to_menu = button_new(18, 11, 24, 3, "MAIN MENU");
    button_set_on_click(btn_to_menu, on_menu_clicked, NULL);
    level_ui_add_button(g_gameover_ui, btn_to_menu);

    struct UIButton *btn_go_quit = button_new(18, 14, 24, 3, "QUIT");
    button_set_on_click(btn_go_quit, on_quit_clicked, NULL);
    level_ui_add_button(g_gameover_ui, btn_go_quit);

    g_game = level_new("Gameplay Arena", 60, 14, '.');
    level_set_default_color(g_game, COLOR_BRIGHT_BLACK, COLOR_DEFAULT);

    level_set_hud_separator(g_game, HUD_TOP, '=', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_set_hud_text(g_game, HUD_TOP, 0, " HP: 100/100  |  SCORE:     0  |  BOSS HP: 30  |  [SPACE] Shoot ", COLOR_BRIGHT_GREEN, COLOR_BLACK);

    level_set_hud_separator(g_game, HUD_BOTTOM, '=', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_set_hud_text(g_game, HUD_BOTTOM, 0, " [ZQSD / Arrows] Move  |  [SPACE] Shoot  |  [ESC] Menu ", COLOR_WHITE, COLOR_BLUE);

    g_mothership = circle_new((int)mothership_x, 2, 4, 'W');
    ellipse_enable_filled(g_mothership);
    ellipse_set_color(g_mothership, COLOR_BRIGHT_MAGENTA, COLOR_DEFAULT);
    level_add_ellipse(g_game, g_mothership);

    g_invader1 = rectangle_new((int)invader1_x, 5, 8, 2, 'M');
    rectangle_enable_filled(g_invader1);
    rectangle_set_color(g_invader1, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_add_rectangle(g_game, g_invader1);

    g_invader2 = rectangle_new((int)invader2_x, 5, 8, 2, 'M');
    rectangle_enable_filled(g_invader2);
    rectangle_set_color(g_invader2, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_add_rectangle(g_game, g_invader2);

    g_player = player_new(30, 12, '^', 18.0f);
    player_set_color(g_player, COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_add_player(g_game, g_player);

    g_engine = engine_new(NULL, 60, 20);
    engine_set_exit_on_escape(g_engine, 0);
    engine_set_level_ui(g_engine, g_menu);
    engine_set_update_callback(g_engine, update);

    engine_run(g_engine, 30);

    engine_free(g_engine);
    return 0;
}
