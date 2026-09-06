#ifndef PLAYER_H
#define PLAYER_H

#include "../../vec2/vec2.h"
#include "../../texture/texture.h"
#include "../../color/color.h"

struct GameObject_Player {
    struct Vector2 position;
    float exact_x;
    float exact_y;
    float speed;
    int health;
    int max_health;
    char character;
    struct Texture *texture;
    Color fg;
    Color bg;
    float vx;
    float vy;
    float jump_power;
    float gravity;
    int use_gravity;
    int is_grounded;
};

struct GameObject_Player* player_new(int x, int y, char character, float speed);
struct GameObject_Player* player_new_textured(int x, int y, struct Texture *texture, float speed);
void player_build(struct GameObject_Player *player, int x, int y, char character, float speed);
void player_free(struct GameObject_Player *player);

void player_move(struct GameObject_Player *player, float dir_x, float dir_y, float delta_time);
void player_set_position(struct GameObject_Player *player, int x, int y);
struct Vector2 player_get_position(struct GameObject_Player *player);
void player_set_speed(struct GameObject_Player *player, float speed);
float player_get_speed(struct GameObject_Player *player);

void player_set_character(struct GameObject_Player *player, char character);
char player_get_character(struct GameObject_Player *player);
void player_set_texture(struct GameObject_Player *player, struct Texture *texture);
struct Texture* player_get_texture(struct GameObject_Player *player);

void player_set_health(struct GameObject_Player *player, int health);
int player_get_health(struct GameObject_Player *player);
void player_take_damage(struct GameObject_Player *player, int amount);
void player_heal(struct GameObject_Player *player, int amount);
int player_is_alive(struct GameObject_Player *player);

void player_set_color(struct GameObject_Player *player, Color fg, Color bg);
Color player_get_fg(struct GameObject_Player *player);
Color player_get_bg(struct GameObject_Player *player);

void player_set_jump_power(struct GameObject_Player *player, float jump_power);
float player_get_jump_power(struct GameObject_Player *player);
void player_set_gravity(struct GameObject_Player *player, float gravity);
float player_get_gravity(struct GameObject_Player *player);
void player_enable_gravity(struct GameObject_Player *player, float gravity);
void player_disable_gravity(struct GameObject_Player *player);
void player_jump(struct GameObject_Player *player);
int player_is_grounded(struct GameObject_Player *player);

#endif
