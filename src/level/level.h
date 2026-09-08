#ifndef LEVEL_H
#define LEVEL_H

#include "../gameobject/point/point.h"
#include "../gameobject/rectangle/rectangle.h"
#include "../gameobject/ellipse/ellipse.h"
#include "../gameobject/player/player.h"
#include "../gameobject/text/text.h"
#include "../projectile/projectile.h"
#include "../particle/particle.h"
#include "../color/color.h"
#include "../light/light.h"

#define MAX_ARRAY_ELEMENTS 500
#define MAX_LEVEL_PROJECTILES 128
#define MAX_LEVEL_LIGHTS 32

typedef enum {
    HUD_TOP = 0,
    HUD_BOTTOM
} HudPosition;

#define MAX_HUD_LINES 8

struct HudLine {
    char text[256];
    Color fg;
    Color bg;
    int is_active;
};

struct Level {
    char* name;
    struct GameObject_Point **points;
    struct GameObject_Rectangle **rectangles;
    struct GameObject_Ellipse **ellipses;
    struct GameObject_Player **players;
    struct GameObject_Text **texts;
    struct GameObject_Projectile *projectiles[MAX_LEVEL_PROJECTILES];
    struct ParticleSystem *particle_system;
    int sizeX;
    int sizeY;
    char defaultCharacter;
    Color default_fg;
    Color default_bg;
    struct HudLine hud_top[MAX_HUD_LINES];
    char hud_top_separator;
    Color hud_top_sep_fg;
    Color hud_top_sep_bg;
    struct HudLine hud_bottom[MAX_HUD_LINES];
    char hud_bottom_separator;
    Color hud_bottom_sep_fg;
    Color hud_bottom_sep_bg;
    int is_camera_enabled;
    int viewport_w;
    int viewport_h;
    float cam_x;
    float cam_y;
    int lighting_enabled;
    Color ambient_light;
    struct Light *lights[MAX_LEVEL_LIGHTS];
    int light_count;
};

struct Level* level_new(char* levelName, int sizeX, int sizeY, char defaultCharacter);
void level_build(struct Level *level, char* levelName, int sizeX, int sizeY, char defaultCharacter);
void level_display(struct Level *level);
void level_add_point(struct Level *level, struct GameObject_Point *point);
void level_remove_point(struct Level *level, struct GameObject_Point *point);
struct GameObject_Point* level_get_point(struct Level *level, struct Vector2 position);
void level_add_rectangle(struct Level *level, struct GameObject_Rectangle *rectangle);
void level_remove_rectangle(struct Level *level, struct GameObject_Rectangle *rectangle);
struct GameObject_Rectangle* level_get_rectangle(struct Level *level, struct Vector2 position);
void level_add_ellipse(struct Level *level, struct GameObject_Ellipse *ellipse);
void level_remove_ellipse(struct Level *level, struct GameObject_Ellipse *ellipse);
struct GameObject_Ellipse* level_get_ellipse(struct Level *level, struct Vector2 position);
void level_add_player(struct Level *level, struct GameObject_Player *player);
void level_remove_player(struct Level *level, struct GameObject_Player *player);
struct GameObject_Player* level_get_player(struct Level *level, struct Vector2 position);
void level_add_text(struct Level *level, struct GameObject_Text *text);
void level_remove_text(struct Level *level, struct GameObject_Text *text);
void level_set_hud_text(struct Level *level, HudPosition pos, int line_index, const char *text, Color fg, Color bg);
void level_clear_hud(struct Level *level, HudPosition pos);
void level_set_hud_separator(struct Level *level, HudPosition pos, char separator_char, Color fg, Color bg);

void level_spawn_projectile(struct Level *level, float x, float y, float vx, float vy, char character, Color fg, int damage, float lifetime, ProjectileOwner owner);
void level_spawn_particles_explosion(struct Level *level, float x, float y, int count, Color fg);
void level_spawn_particles_sparkle(struct Level *level, float x, float y, int count, Color fg);
void level_update(struct Level *level, float dt);
char* level_get_name(struct Level *level);
int level_get_sizeX(struct Level *level);
int level_get_sizeY(struct Level *level);
char level_get_defaultCharacter(struct Level *level);
void level_set_name(struct Level *level, char* name);
void level_set_sizeX(struct Level *level, int sizeX);
void level_set_sizeY(struct Level *level, int sizeY);
void level_set_defaultCharacter(struct Level *level, char defaultCharacter);
void level_set_default_color(struct Level *level, Color fg, Color bg);
Color level_get_default_fg(struct Level *level);
Color level_get_default_bg(struct Level *level);
void level_set_camera(struct Level *level, float cam_x, float cam_y);
void level_enable_camera(struct Level *level, int viewport_w, int viewport_h);
void level_disable_camera(struct Level *level);
void level_free(struct Level *level);
void level_reset_screen_buffer(void);

void level_enable_lighting(struct Level *level, int enabled);
void level_set_ambient_light(struct Level *level, Color ambient);
void level_add_light(struct Level *level, struct Light *light);
void level_remove_light(struct Level *level, struct Light *light);
void level_clear_lights(struct Level *level);
int level_is_solid_at(struct Level *level, int x, int y);

#endif