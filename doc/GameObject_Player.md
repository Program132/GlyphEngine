# GameObject_Player

The `GameObject_Player` structure represents an interactive player entity within a `Level`. It supports smooth sub-pixel delta-time movement, directional motion, health tracking, single-character glyphs, and multi-character ASCII `Texture` sprites with transparency.

# Struct

| Argument   | Type            | Description                                                                     |
|------------|-----------------|---------------------------------------------------------------------------------|
| position   | struct Vector2  | Integer grid coordinates of the player.                                         |
| exact_x    | float           | Sub-pixel X accumulator for smooth floating-point delta-time movement.          |
| exact_y    | float           | Sub-pixel Y accumulator for smooth floating-point delta-time movement.          |
| speed      | float           | Movement speed in units per second.                                             |
| health     | int             | Current vitality of the player.                                                 |
| max_health | int             | Maximum possible health of the player.                                          |
| character  | char            | Single character glyph (used when no texture is assigned).                      |
| texture    | struct Texture* | Pointer to an optional multi-character ASCII sprite with transparency.          |
| fg         | Color           | Foreground ANSI color.                                                          |
| bg         | Color           | Background ANSI color.                                                          |

# Functions

| Name                 | Type                      | Arguments                                                                | Description                                                                               |
|----------------------|---------------------------|--------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|
| player_new           | struct GameObject_Player* | int x, int y, char character, float speed                                | Dynamically allocates and initializes a player with a single-character glyph.             |
| player_new_textured  | struct GameObject_Player* | int x, int y, struct Texture *texture, float speed                       | Dynamically allocates and initializes a player with a multi-character ASCII texture.      |
| player_build         | void                      | struct GameObject_Player *player, int x, int y, char char, float speed   | Initializes the fields of an existing player instance.                                    |
| player_free          | void                      | struct GameObject_Player *player                                         | Frees the player and its texture (if assigned).                                           |
| player_move          | void                      | struct GameObject_Player *player, float dir_x, float dir_y, float dt     | Moves the player along a normalized direction vector scaled by its speed and delta time.  |
| player_set_position  | void                      | struct GameObject_Player *player, int x, int y                           | Teleports the player to the specified integer coordinates.                                |
| player_get_position  | struct Vector2            | struct GameObject_Player *player                                         | Returns the current grid position (`Vector2`) of the player.                              |
| player_set_speed     | void                      | struct GameObject_Player *player, float speed                            | Updates the player's movement speed.                                                      |
| player_get_speed     | float                     | struct GameObject_Player *player                                         | Returns the player's current speed.                                                       |
| player_set_character | void                      | struct GameObject_Player *player, char character                         | Sets the single-character glyph of the player.                                            |
| player_get_character | char                      | struct GameObject_Player *player                                         | Returns the player's character glyph.                                                     |
| player_set_texture   | void                      | struct GameObject_Player *player, struct Texture *texture                | Assigns an ASCII texture sprite to the player.                                            |
| player_get_texture   | struct Texture*           | struct GameObject_Player *player                                         | Returns the player's texture (or `NULL`).                                                 |
| player_set_health    | void                      | struct GameObject_Player *player, int health                             | Sets the player's current health.                                                         |
| player_get_health    | int                       | struct GameObject_Player *player                                         | Returns the current health value.                                                         |
| player_take_damage   | void                      | struct GameObject_Player *player, int amount                             | Reduces player health by the specified amount (cannot drop below 0).                      |
| player_heal          | void                      | struct GameObject_Player *player, int amount                             | Increases player health up to `max_health`.                                               |
| player_is_alive      | int                       | struct GameObject_Player *player                                         | Returns `1` if health > 0, `0` otherwise.                                                 |
| player_set_color     | void                      | struct GameObject_Player *player, Color fg, Color bg                     | Sets the foreground and background ANSI colors for the player.                            |
| player_get_fg        | Color                     | struct GameObject_Player *player                                         | Returns the foreground color.                                                             |
| player_get_bg        | Color                     | struct GameObject_Player *player                                         | Returns the background color.                                                             |
