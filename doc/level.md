# Struct

| Argument         | Type                          | Description                                         |
|------------------|-------------------------------|-----------------------------------------------------|
| name             | char*                         | The name of the level/scene                         |
| points           | struct GameObject_Point**     | Dynamic array of pointers to points                 |
| rectangles       | struct GameObject_Rectangle** | Dynamic array of pointers to rectangles             |
| ellipses         | struct GameObject_Ellipse**   | Dynamic array of pointers to ellipses               |
| players          | struct GameObject_Player**    | Dynamic array of pointers to players                |
| texts            | struct GameObject_Text**      | Dynamic array of pointers to text/HUD objects       |
| projectiles      | struct GameObject_Projectile**| Dynamic array of pointers to active projectiles     |
| particle_system  | struct ParticleSystem*        | Integrated particle pool system                     |
| sizeX            | int                           | Level width (number of columns)                     |
| sizeY            | int                           | Level height (number of rows)                       |
| defaultCharacter | char                          | Default character used for empty/background cells   |
| default_fg       | Color                         | Default foreground color for background cells       |
| default_bg       | Color                         | Default background color for background cells       |
| is_camera_enabled| int                           | Flag indicating whether viewport/camera is enabled  |
| viewport_w       | int                           | Width of visible camera viewport on console         |
| viewport_h       | int                           | Height of visible camera viewport on console        |
| cam_x            | float                         | Top-left world X coordinate of camera viewport      |
| cam_y            | float                         | Top-left world Y coordinate of camera viewport      |

# Constants

| Name               | Value | Description                                          |
|--------------------|-------|------------------------------------------------------|
| MAX_ARRAY_ELEMENTS | 500   | Maximum number of elements that can be stored per list |

# Functions

| Name                       | Type                         | Arguments                                                                               | Description                                                                               |
|----------------------------|------------------------------|-----------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|
| level_new                  | struct Level*                | char* levelName, int sizeX, int sizeY, char defaultCharacter                            | Dynamically allocates and initializes a new level with the specified parameters          |
| level_build                | void                         | struct Level *level, char* levelName, int sizeX, int sizeY, char defaultCharacter       | Initializes an existing level structure, allocates pointer arrays, and sets fields         |
| level_display              | void                         | struct Level *level                                                                     | Renders the level using an optimized double buffer with ANSI color tracking               |
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
| level_add_text             | void                         | struct Level *level, struct GameObject_Text *text                                       | Adds a text object / HUD label to the level (level assumes ownership of memory)          |
| level_remove_text          | void                         | struct Level *level, struct GameObject_Text *text                                       | Removes the specified text object from the level                                          |
| level_set_hud_text         | void                         | struct Level *level, HudPosition pos, int line_index, const char *text, Color fg, Color bg | Sets an off-canvas HUD text line above (HUD_TOP) or below (HUD_BOTTOM) the level grid.   |
| level_clear_hud            | void                         | struct Level *level, HudPosition pos                                                     | Clears all off-canvas HUD lines at the specified position.                                |
| level_set_hud_separator    | void                         | struct Level *level, HudPosition pos, char separator_char, Color fg, Color bg            | Sets a full-width separator line between the off-canvas HUD and the game level grid.      |
| level_spawn_projectile     | void                         | struct Level *level, float x, float y, float vx, float vy, char character, Color fg, int damage, float lifetime, ProjectileOwner owner | Spawns a projectile into the level's projectile pool.                                  |
| level_spawn_particles_explosion | void                    | struct Level *level, float x, float y, int count, Color fg                                | Spawns an explosion particle burst at the given position.                                 |
| level_spawn_particles_sparkle   | void                    | struct Level *level, float x, float y, int count, Color fg                                | Spawns a sparkle particle burst at the given position.                                   |
| level_update               | void                         | struct Level *level, float dt                                                           | Updates all active projectiles and particles by dt seconds.                               |
| level_get_name             | char*                        | struct Level *level                                                                     | Returns the name of the level                                                             |
| level_get_sizeX            | int                          | struct Level *level                                                                     | Returns the width (sizeX) of the level                                                    |
| level_get_sizeY            | int                          | struct Level *level                                                                     | Returns the height (sizeY) of the level                                                   |
| level_get_defaultCharacter | char                         | struct Level *level                                                                     | Returns the default background character                                                  |
| level_set_name             | void                         | struct Level *level, char* name                                                         | Sets the level name                                                                       |
| level_set_sizeX            | void                         | struct Level *level, int sizeX                                                          | Sets the width (sizeX) of the level                                                       |
| level_set_sizeY            | void                         | struct Level *level, int sizeY                                                          | Sets the height (sizeY) of the level                                                      |
| level_set_defaultCharacter | void                         | struct Level *level, char defaultCharacter                                             | Sets the default background character                                                     |
| level_set_default_color    | void                         | struct Level *level, Color fg, Color bg                                                 | Sets the default foreground and background colors for background cells                     |
| level_get_default_fg       | Color                        | struct Level *level                                                                     | Returns the default foreground color                                                      |
| level_get_default_bg       | Color                        | struct Level *level                                                                     | Returns the default background color                                                      |
| level_set_camera           | void                         | struct Level *level, float cam_x, float cam_y                                           | Sets the top-left coordinate of the camera viewport.                                     |
| level_enable_camera        | void                         | struct Level *level, int viewport_w, int viewport_h                                     | Enables viewport camera rendering with the given window dimensions.                       |
| level_disable_camera       | void                         | struct Level *level                                                                     | Disables camera mode and restores standard whole-level rendering.                         |
| level_free                 | void                         | struct Level *level                                                                     | Frees the level, its internal arrays, and all contained game objects (`*_free`)          |

## Memory Management
When objects (`GameObject_Point`, `GameObject_Rectangle`, `GameObject_Ellipse`, `GameObject_Player`) are added via `level_add_*`, the `Level` assumes ownership of their memory. Calling `level_free(level)` automatically calls the respective cleanup function on every contained object before freeing the level's arrays and structure.
