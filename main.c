#include "src/engine/engine.h"
#include "src/inputs/inputs.h"
#include "src/levelui/levelui.h"
#include "src/ui/button/button.h"
#include "src/ui/panel/panel.h"
#include "src/gameobject/text/text.h"
#include "src/gameobject/player/player.h"
#include "src/gameobject/rectangle/rectangle.h"
#include "src/gameobject/circle/circle.h"
#include "src/gameobject/point/point.h"
#include "src/collision/collision.h"
#include "src/color/color.h"
#include <stdio.h>

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
struct GameObject_Rectangle *g_lava = NULL;
struct GameObject_Ellipse *g_orb1 = NULL;
struct GameObject_Ellipse *g_orb2 = NULL;
struct GameObject_Point *g_coin = NULL;
struct GameObject_Text *g_final_score = NULL;

enum GameState g_state = STATE_MENU;
int g_score = 0;
float orb1_x = 20.0f, orb1_y = 4.0f, orb1_dx = 16.0f, orb1_dy = 9.0f;
float orb2_x = 40.0f, orb2_y = 8.0f, orb2_dx = -12.0f, orb2_dy = -11.0f;

void reset_game(void) {
    g_score = 0;
    player_set_health(g_player, 100);
    player_set_position(g_player, 10, 6);
    g_player->exact_x = 10.0f;
    g_player->exact_y = 6.0f;
    player_set_color(g_player, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    orb1_x = 20.0f; orb1_y = 4.0f; orb1_dx = 16.0f; orb1_dy = 9.0f;
    orb2_x = 40.0f; orb2_y = 8.0f; orb2_dx = -12.0f; orb2_dy = -11.0f;
    g_coin->position.x = 45;
    g_coin->position.y = 5;
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
    int esc = input_is_key_down(KEY_ESCAPE);

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

    orb1_x += orb1_dx * dt;
    orb1_y += orb1_dy * dt;
    if (orb1_x <= 2.0f || orb1_x >= g_game->sizeX - 6.0f) orb1_dx = -orb1_dx;
    if (orb1_y <= 1.0f || orb1_y >= g_game->sizeY - 4.0f) orb1_dy = -orb1_dy;
    g_orb1->position.x = (int)orb1_x;
    g_orb1->position.y = (int)orb1_y;

    orb2_x += orb2_dx * dt;
    orb2_y += orb2_dy * dt;
    if (orb2_x <= 2.0f || orb2_x >= g_game->sizeX - 6.0f) orb2_dx = -orb2_dx;
    if (orb2_y <= 1.0f || orb2_y >= g_game->sizeY - 4.0f) orb2_dy = -orb2_dy;
    g_orb2->position.x = (int)orb2_x;
    g_orb2->position.y = (int)orb2_y;

    int took_damage = 0;
    if (collision_check_player_ellipse(g_player, g_orb1)) {
        player_take_damage(g_player, 2);
        took_damage = 1;
    }
    if (collision_check_player_ellipse(g_player, g_orb2)) {
        player_take_damage(g_player, 2);
        took_damage = 1;
    }
    if (collision_check_player_rect(g_player, g_lava)) {
        player_take_damage(g_player, 3);
        took_damage = 1;
    }

    if (took_damage) {
        player_set_color(g_player, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    } else {
        player_set_color(g_player, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    }

    if (g_coin != NULL && collision_check_player_point(g_player, g_coin)) {
        g_score += 100;
        player_heal(g_player, 15);
        g_coin->position.x = 6 + (g_coin->position.x * 7) % 48;
        g_coin->position.y = 2 + (g_coin->position.y * 3) % 8;
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
    snprintf(top_buf, sizeof(top_buf), " HP: %3d/100   |   SCORE: %5d   |   DODGE THE RED ORBS! ", hp, g_score);
    Color hud_color = (hp > 50) ? COLOR_BRIGHT_GREEN : (hp > 20 ? COLOR_BRIGHT_YELLOW : COLOR_BRIGHT_RED);
    level_set_hud_text(g_game, HUD_TOP, 0, top_buf, hud_color, COLOR_BLACK);
}

int main() {
    g_menu = level_ui_new("Main Menu", 60, 20, ' ');
    level_ui_set_default_color(g_menu, COLOR_WHITE, COLOR_DEFAULT);

    struct UIPanel *box = panel_new_styled(14, 2, 32, 16, " GLYPH ENGINE ", '#', ' ', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_ui_add_panel(g_menu, box);

    struct GameObject_Text *sub = text_new_colored(19, 4, "Terminal GUI & Menu", COLOR_YELLOW, COLOR_DEFAULT);
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
    level_set_hud_text(g_game, HUD_TOP, 0, " HP: 100/100   |   SCORE:     0   |   DODGE THE RED ORBS! ", COLOR_BRIGHT_GREEN, COLOR_BLACK);

    level_set_hud_separator(g_game, HUD_BOTTOM, '=', COLOR_BRIGHT_CYAN, COLOR_DEFAULT);
    level_set_hud_text(g_game, HUD_BOTTOM, 0, " [ZQSD / Arrows] Move   |   [ESC] Return to Menu ", COLOR_WHITE, COLOR_BLUE);

    g_lava = rectangle_new(20, 10, 20, 3, '~');
    rectangle_enable_filled(g_lava);
    rectangle_set_color(g_lava, COLOR_BRIGHT_RED, COLOR_RED);
    level_add_rectangle(g_game, g_lava);

    g_orb1 = circle_new((int)orb1_x, (int)orb1_y, 4, 'O');
    ellipse_enable_filled(g_orb1);
    ellipse_set_color(g_orb1, COLOR_BRIGHT_RED, COLOR_DEFAULT);
    level_add_ellipse(g_game, g_orb1);

    g_orb2 = circle_new((int)orb2_x, (int)orb2_y, 5, 'X');
    ellipse_enable_filled(g_orb2);
    ellipse_set_color(g_orb2, COLOR_BRIGHT_MAGENTA, COLOR_DEFAULT);
    level_add_ellipse(g_game, g_orb2);

    g_coin = point_new(45, 5, '$');
    point_set_color(g_coin, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_point(g_game, g_coin);

    g_player = player_new(10, 6, '@', 16.0f);
    player_set_color(g_player, COLOR_BRIGHT_YELLOW, COLOR_DEFAULT);
    level_add_player(g_game, g_player);

    g_engine = engine_new(NULL, 60, 20);
    engine_set_exit_on_escape(g_engine, 0);
    engine_set_level_ui(g_engine, g_menu);
    engine_set_update_callback(g_engine, update);

    engine_run(g_engine, 30);

    engine_free(g_engine);
    return 0;
}
