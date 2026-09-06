#ifndef LEVEL_H
#define LEVEL_H

#include "../gameobject/point/point.h"
#include "../gameobject/rectangle/rectangle.h"
#include "../gameobject/ellipse/ellipse.h"
#include "../gameobject/player/player.h"
#include "../gameobject/text/text.h"
#include "../color/color.h"

#define MAX_ARRAY_ELEMENTS 500

struct Level {
    char* name;
    struct GameObject_Point **points;
    struct GameObject_Rectangle **rectangles;
    struct GameObject_Ellipse **ellipses;
    struct GameObject_Player **players;
    struct GameObject_Text **texts;
    int sizeX;
    int sizeY;
    char defaultCharacter;
    Color default_fg;
    Color default_bg;
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
void level_free(struct Level *level);

#endif