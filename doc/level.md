# Struct

| Argument         | Type                          | Description                                         |
|------------------|-------------------------------|-----------------------------------------------------|
| name             | char*                         | The name of the level/scene                         |
| points           | struct GameObject_Point**     | Dynamic array of pointers to points                 |
| rectangles       | struct GameObject_Rectangle** | Dynamic array of pointers to rectangles             |
| ellipses         | struct GameObject_Ellipse**   | Dynamic array of pointers to ellipses               |
| players          | struct GameObject_Player**    | Dynamic array of pointers to players                |
| sizeX            | int                           | Level width (number of columns)                     |
| sizeY            | int                           | Level height (number of rows)                       |
| defaultCharacter | char                          | Default character used for empty/background cells   |

# Constants

| Name               | Value | Description                                          |
|--------------------|-------|------------------------------------------------------|
| MAX_ARRAY_ELEMENTS | 500   | Maximum number of elements that can be stored per list |

# Functions

| Name                       | Type                         | Arguments                                                                               | Description                                                                               |
|----------------------------|------------------------------|-----------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|
| level_new                  | struct Level*                | char* levelName, int sizeX, int sizeY, char defaultCharacter                            | Dynamically allocates and initializes a new level with the specified parameters          |
| level_build                | void                         | struct Level *level, char* levelName, int sizeX, int sizeY, char defaultCharacter       | Initializes an existing level structure, allocates pointer arrays, and sets fields         |
| level_display              | void                         | struct Level *level                                                                     | Renders the level using a single frame buffer to avoid console flickering                 |
| level_add_point            | void                         | struct Level *level, struct GameObject_Point *point                                     | Adds a point to the level (the level assumes ownership of the object's memory)             |
| level_remove_point         | void                         | struct Level *level, struct GameObject_Point *point                                     | Removes the specified point from the level                                                |
| level_get_point            | struct GameObject_Point*     | struct Level *level, struct Vector2 position                                            | Returns a pointer to the point at the given position (or NULL if not found)              |
| level_add_rectangle        | void                         | struct Level *level, struct GameObject_Rectangle *rectangle                             | Adds a rectangle to the level (the level assumes ownership of the memory)                 |
| level_remove_rectangle     | void                         | struct Level *level, struct GameObject_Rectangle *rectangle                             | Removes the specified rectangle from the level                                            |
| level_get_rectangle        | struct GameObject_Rectangle* | struct Level *level, struct Vector2 position                                            | Returns a pointer to the rectangle covering the given position (or NULL)                  |
| level_add_ellipse          | void                         | struct Level *level, struct GameObject_Ellipse *ellipse                                 | Adds an ellipse to the level (the level assumes ownership of the memory)                   |
| level_remove_ellipse       | void                         | struct Level *level, struct GameObject_Ellipse *ellipse                                 | Removes the specified ellipse from the level                                              |
| level_get_ellipse          | struct GameObject_Ellipse*   | struct Level *level, struct Vector2 position                                            | Returns a pointer to the ellipse covering the given position (or NULL)                    |
| level_add_player           | void                         | struct Level *level, struct GameObject_Player *player                                   | Adds a player entity to the level (the level assumes ownership of the player)            |
| level_remove_player        | void                         | struct Level *level, struct GameObject_Player *player                                   | Removes the specified player entity from the level                                        |
| level_get_player           | struct GameObject_Player*    | struct Level *level, struct Vector2 position                                            | Returns a pointer to the player at or covering the given position (or NULL)               |
| level_get_name             | char*                        | struct Level *level                                                                     | Returns the name of the level                                                             |
| level_get_sizeX            | int                          | struct Level *level                                                                     | Returns the width (sizeX) of the level                                                    |
| level_get_sizeY            | int                          | struct Level *level                                                                     | Returns the height (sizeY) of the level                                                   |
| level_get_defaultCharacter | char                         | struct Level *level                                                                     | Returns the default background character                                                  |
| level_set_name             | void                         | struct Level *level, char* name                                                         | Sets the level name                                                                       |
| level_set_sizeX            | void                         | struct Level *level, int sizeX                                                          | Sets the width (sizeX) of the level                                                       |
| level_set_sizeY            | void                         | struct Level *level, int sizeY                                                          | Sets the height (sizeY) of the level                                                      |
| level_set_defaultCharacter | void                         | struct Level *level, char defaultCharacter                                             | Sets the default background character                                                     |
| level_free                 | void                         | struct Level *level                                                                     | Frees the level, its internal arrays, and all contained game objects (`*_free`)          |

## Memory Management
When objects (`GameObject_Point`, `GameObject_Rectangle`, `GameObject_Ellipse`, `GameObject_Player`) are added via `level_add_*`, the `Level` assumes ownership of their memory. Calling `level_free(level)` automatically calls the respective cleanup function on every contained object before freeing the level's arrays and structure.
