#include "player.h"
#include <stdlib.h>

struct GameObject_Player* player_new(int x, int y, char character, float speed) {
    struct GameObject_Player *player = malloc(sizeof(struct GameObject_Player));
    if (player == NULL) return NULL;
    player_build(player, x, y, character, speed);
    return player;
}

struct GameObject_Player* player_new_textured(int x, int y, struct Texture *texture, float speed) {
    struct GameObject_Player *player = malloc(sizeof(struct GameObject_Player));
    if (player == NULL) return NULL;
    player_build(player, x, y, '\0', speed);
    player->texture = texture;
    return player;
}

void player_build(struct GameObject_Player *player, int x, int y, char character, float speed) {
    player->position.x = x;
    player->position.y = y;
    player->exact_x = (float)x;
    player->exact_y = (float)y;
    player->speed = speed;
    player->character = character;
    player->texture = NULL;
    player->health = 100;
    player->max_health = 100;
    player->fg = COLOR_DEFAULT;
    player->bg = COLOR_DEFAULT;
    player->vx = 0.0f;
    player->vy = 0.0f;
    player->jump_power = 18.0f;
    player->gravity = 45.0f;
    player->use_gravity = 0;
    player->is_grounded = 0;
}

void player_free(struct GameObject_Player *player) {
    if (player == NULL) return;
    if (player->texture != NULL) {
        texture_free(player->texture);
        player->texture = NULL;
    }
    free(player);
}

void player_move(struct GameObject_Player *player, float dir_x, float dir_y, float delta_time) {
    if (player == NULL) return;
    player->exact_x += dir_x * player->speed * delta_time;
    player->exact_y += dir_y * player->speed * delta_time;
    player->position.x = (int)(player->exact_x + 0.5f);
    player->position.y = (int)(player->exact_y + 0.5f);
}

void player_set_position(struct GameObject_Player *player, int x, int y) {
    if (player == NULL) return;
    player->position.x = x;
    player->position.y = y;
    player->exact_x = (float)x;
    player->exact_y = (float)y;
}

struct Vector2 player_get_position(struct GameObject_Player *player) {
    if (player == NULL) {
        struct Vector2 zero = {0, 0};
        return zero;
    }
    return player->position;
}

void player_set_speed(struct GameObject_Player *player, float speed) {
    if (player == NULL) return;
    player->speed = speed;
}

float player_get_speed(struct GameObject_Player *player) {
    if (player == NULL) return 0.0f;
    return player->speed;
}

void player_set_character(struct GameObject_Player *player, char character) {
    if (player == NULL) return;
    player->character = character;
}

char player_get_character(struct GameObject_Player *player) {
    if (player == NULL) return '\0';
    return player->character;
}

void player_set_texture(struct GameObject_Player *player, struct Texture *texture) {
    if (player == NULL) return;
    player->texture = texture;
}

struct Texture* player_get_texture(struct GameObject_Player *player) {
    if (player == NULL) return NULL;
    return player->texture;
}

void player_set_health(struct GameObject_Player *player, int health) {
    if (player == NULL) return;
    player->health = health;
}

int player_get_health(struct GameObject_Player *player) {
    if (player == NULL) return 0;
    return player->health;
}

void player_take_damage(struct GameObject_Player *player, int amount) {
    if (player == NULL) return;
    player->health -= amount;
    if (player->health < 0) player->health = 0;
}

void player_heal(struct GameObject_Player *player, int amount) {
    if (player == NULL) return;
    player->health += amount;
    if (player->health > player->max_health) player->health = player->max_health;
}

int player_is_alive(struct GameObject_Player *player) {
    if (player == NULL) return 0;
    return player->health > 0;
}

void player_set_color(struct GameObject_Player *player, Color fg, Color bg) {
    if (player == NULL) return;
    player->fg = fg;
    player->bg = bg;
}

Color player_get_fg(struct GameObject_Player *player) {
    if (player == NULL) return COLOR_DEFAULT;
    return player->fg;
}

Color player_get_bg(struct GameObject_Player *player) {
    if (player == NULL) return COLOR_DEFAULT;
    return player->bg;
}

void player_set_jump_power(struct GameObject_Player *player, float jump_power) {
    if (player == NULL) return;
    player->jump_power = jump_power;
}

float player_get_jump_power(struct GameObject_Player *player) {
    if (player == NULL) return 0.0f;
    return player->jump_power;
}

void player_set_gravity(struct GameObject_Player *player, float gravity) {
    if (player == NULL) return;
    player->gravity = gravity;
}

float player_get_gravity(struct GameObject_Player *player) {
    if (player == NULL) return 0.0f;
    return player->gravity;
}

void player_enable_gravity(struct GameObject_Player *player, float gravity) {
    if (player == NULL) return;
    player->use_gravity = 1;
    player->gravity = gravity;
}

void player_disable_gravity(struct GameObject_Player *player) {
    if (player == NULL) return;
    player->use_gravity = 0;
    player->vy = 0.0f;
}

void player_jump(struct GameObject_Player *player) {
    if (player == NULL) return;
    if (player->is_grounded) {
        player->vy = -player->jump_power;
        player->is_grounded = 0;
    }
}

int player_is_grounded(struct GameObject_Player *player) {
    if (player == NULL) return 0;
    return player->is_grounded;
}
