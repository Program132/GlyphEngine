# Struct

| Argument  | Type            | Description                                        |
|-----------|-----------------|----------------------------------------------------|
| position  | struct Vector2  | 2D position of the point                           |
| character | char            | Character displayed in the terminal for this point |
| fg        | Color           | Foreground ANSI color                              |
| bg        | Color           | Background ANSI color                              |

# Functions

| Name                 | Type                      | Arguments                                                     | Description                                                                  |
|----------------------|---------------------------|---------------------------------------------------------------|------------------------------------------------------------------------------|
| point_new            | struct GameObject_Point*  | int x, int y, char character                                  | Dynamically allocates and initializes a new point at coordinates (x, y).      |
| point_build          | void                      | struct GameObject_Point *point, int x, int y, char character  | Initializes the fields of an existing GameObject_Point instance.             |
| point_free           | void                      | struct GameObject_Point *point                                | Frees dynamically allocated memory for the point.                            |
| point_get_position   | struct Vector2            | struct GameObject_Point *point                                | Returns the current position (Vector2) of the point.                         |
| point_set_position   | void                      | struct GameObject_Point *point, struct Vector2 newPosition    | Sets the position of the point to the given 2D vector.                       |
| point_get_character  | char                      | struct GameObject_Point *point                                | Returns the display character of the point.                                  |
| point_set_character  | void                      | struct GameObject_Point *point, char newCharacter             | Sets the display character of the point.                                     |
| point_set_color      | void                      | struct GameObject_Point *point, Color fg, Color bg            | Sets the foreground and background ANSI colors for the point.                 |
| point_get_fg         | Color                     | struct GameObject_Point *point                                | Returns the foreground color.                                                |
| point_get_bg         | Color                     | struct GameObject_Point *point                                | Returns the background color.                                                |