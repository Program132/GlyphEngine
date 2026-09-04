#ifndef LEVEL_H
#define LEVEL_H

#include "../gameobject/point/point.h"
#include "../gameobject/rectangle/rectangle.h"
#include "../gameobject/ellipse/ellipse.h"

#define MAX_ARRAY_ELEMENTS 500

struct Level {
    char* name;
    struct GameObject_Point **points;
    struct GameObject_Rectangle **rectangles;
    struct GameObject_Ellipse **ellipses;
    int sizeX;
    int sizeY;
    char defaultCharacter;
};

struct Level* level_new(char* levelName, int sizeX, int sizeY, char defaultCharacter);
void level_build(struct Level *level, char* levelName, int sizeX, int sizeY, char defaultCharacter);
void level_display(struct Level *level);
void level_add_point(struct Level *level, struct GameObject_Point *point);
void level_remove_point(struct Level *level, struct GameObject_Point *point);
struct GameObject_Point* level_get_point(struct Level *level, struct Vector2 position);
void level_add_rectangle(struct Level *level, struct GameObject_Rectangle *rectangle);
void level_remove_rectangle(struct Level *level, struct GameObject_Rectangle *rectangle);

/**
 * @brief Retrieves the first rectangle object found at the given position.
 * @param level Pointer to the Level structure.
 * @param position Position to check for a rectangle.
 * @return Pointer to the GameObject_Rectangle found, or NULL if none exists at this position.
 */
struct GameObject_Rectangle* level_get_rectangle(struct Level *level, struct Vector2 position);

/**
 * @brief Adds an ellipse object to the level's rendering list.
 * @param level Pointer to the Level structure.
 * @param ellipse Pointer to the GameObject_Ellipse to add.
 */
void level_add_ellipse(struct Level *level, struct GameObject_Ellipse *ellipse);

/**
 * @brief Removes an ellipse object from the level's rendering list.
 * @param level Pointer to the Level structure.
 * @param ellipse Pointer to the GameObject_Ellipse to remove.
 */
void level_remove_ellipse(struct Level *level, struct GameObject_Ellipse *ellipse);

/**
 * @brief Retrieves the first ellipse object found at the given position.
 * @param level Pointer to the Level structure.
 * @param position Position to check for an ellipse.
 * @return Pointer to the GameObject_Ellipse found, or NULL if none exists at this position.
 */
struct GameObject_Ellipse* level_get_ellipse(struct Level *level, struct Vector2 position);

char* level_get_name(struct Level *level);
int level_get_sizeX(struct Level *level);
int level_get_sizeY(struct Level *level);
char level_get_defaultCharacter(struct Level *level);
void level_set_name(struct Level *level, char* name);
void level_set_sizeX(struct Level *level, int sizeX);
void level_set_sizeY(struct Level *level, int sizeY);
void level_set_defaultCharacter(struct Level *level, char defaultCharacter);
void level_free(struct Level *level);

#endif