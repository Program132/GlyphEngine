# Struct

| Argument  | Type            | Description                                                                     |
|-----------|-----------------|---------------------------------------------------------------------------------|
| position  | struct Vector2  | Position (top-left corner of bounding box) of the ellipse in 2D space.          |
| width     | int             | Width of the ellipse.                                                           |
| height    | int             | Height of the ellipse.                                                          |
| character | char            | Character used to render the ellipse when untextured.                           |
| filled    | int             | If 1, the ellipse is filled. If 0, only the outline is rendered.                 |
| texture   | struct Texture* | Optional ASCII texture pattern repeated across the ellipse's interior or border. |
| fg        | Color           | Foreground ANSI color.                                                          |
| bg        | Color           | Background ANSI color.                                                          |

# Functions

| Name                   | Type                       | Arguments                                                                                | Description                                                                                 |
|------------------------|----------------------------|------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|
| ellipse_new            | struct GameObject_Ellipse* | int x, int y, int width, int height, char character                                      | Dynamically allocates and initializes a new ellipse with the given dimensions.              |
| ellipse_new_textured   | struct GameObject_Ellipse* | int x, int y, int width, int height, struct Texture *texture                             | Dynamically allocates and initializes a new textured ellipse.                              |
| ellipse_build          | void                       | struct GameObject_Ellipse *ellipse, int x, int y, int width, int height, char character  | Initializes the fields of an existing GameObject_Ellipse instance.                         |
| ellipse_free           | void                       | struct GameObject_Ellipse *ellipse                                                       | Frees dynamically allocated memory for the ellipse.                                         |
| ellipse_get_position   | struct Vector2             | struct GameObject_Ellipse *ellipse                                                       | Returns the current position (Vector2) of the ellipse.                                      |
| ellipse_set_position   | void                       | struct GameObject_Ellipse *ellipse, struct Vector2 newPosition                           | Sets the position of the ellipse.                                                           |
| ellipse_get_width      | int                        | struct GameObject_Ellipse *ellipse                                                       | Returns the width of the ellipse.                                                           |
| ellipse_set_width      | void                       | struct GameObject_Ellipse *ellipse, int newWidth                                         | Sets the width of the ellipse.                                                              |
| ellipse_get_height     | int                        | struct GameObject_Ellipse *ellipse                                                       | Returns the height of the ellipse.                                                          |
| ellipse_set_height     | void                       | struct GameObject_Ellipse *ellipse, int newHeight                                        | Sets the height of the ellipse.                                                             |
| ellipse_get_character  | char                       | struct GameObject_Ellipse *ellipse                                                       | Returns the display character of the ellipse.                                               |
| ellipse_set_character  | void                       | struct GameObject_Ellipse *ellipse, char newCharacter                                    | Sets the display character of the ellipse.                                                  |
| ellipse_enable_filled  | void                       | struct GameObject_Ellipse *ellipse                                                       | Enables filled mode (`filled` = 1).                                                         |
| ellipse_disable_filled | void                       | struct GameObject_Ellipse *ellipse                                                       | Disables filled mode (`filled` = 0).                                                        |
| ellipse_set_texture    | void                       | struct GameObject_Ellipse *ellipse, struct Texture *texture                              | Assigns an ASCII pattern texture to the ellipse.                                            |
| ellipse_get_texture    | struct Texture*            | struct GameObject_Ellipse *ellipse                                                       | Returns the assigned texture (or `NULL`).                                                   |
| ellipse_set_color      | void                       | struct GameObject_Ellipse *ellipse, Color fg, Color bg                                   | Sets the foreground and background ANSI colors for the ellipse.                            |
| ellipse_get_fg         | Color                      | struct GameObject_Ellipse *ellipse                                                       | Returns the foreground color.                                                               |
| ellipse_get_bg         | Color                      | struct GameObject_Ellipse *ellipse                                                       | Returns the background color.                                                               |
| ellipse_get_points     | struct GameObject_Point*   | struct GameObject_Ellipse *ellipse                                                       | Allocates and returns an array of all `GameObject_Point` belonging to the ellipse.          |
