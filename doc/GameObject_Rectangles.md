# Struct

| Argument  | Type           | Description                                                        |
|-----------|----------------|--------------------------------------------------------------------|
| position  | struct Vector2 | Position (top-left corner) of the rectangle in 2D space            |
| width     | int            | Width of the rectangle                                             |
| height    | int            | Height of the rectangle                                            |
| character | char           | Character used to render the rectangle                             |
| filled    | int            | If 1, the rectangle is filled. If 0, only the outline is rendered. |

# Functions

| Name                     | Type                          | Arguments                                                                                     | Description                                                                                 |
|--------------------------|-------------------------------|-----------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|
| rectangle_new            | struct GameObject_Rectangle*  | int x, int y, int width, int height, char character                                           | Dynamically allocates and initializes a new rectangle with the given dimensions.            |
| rectangle_build          | void                          | struct GameObject_Rectangle *rectangle, int x, int y, int width, int height, char character     | Initializes the fields of an existing GameObject_Rectangle instance.                       |
| rectangle_free           | void                          | struct GameObject_Rectangle *rectangle                                                        | Frees dynamically allocated memory for the rectangle.                                       |
| rectangle_get_position   | struct Vector2                | struct GameObject_Rectangle *rectangle                                                        | Returns the current position (Vector2) of the rectangle.                                    |
| rectangle_set_position   | void                          | struct GameObject_Rectangle *rectangle, struct Vector2 newPosition                            | Sets the position of the rectangle.                                                         |
| rectangle_get_width      | int                           | struct GameObject_Rectangle *rectangle                                                        | Returns the width of the rectangle.                                                         |
| rectangle_set_width      | void                          | struct GameObject_Rectangle *rectangle, int newWidth                                          | Sets the width of the rectangle.                                                            |
| rectangle_get_height     | int                           | struct GameObject_Rectangle *rectangle                                                        | Returns the height of the rectangle.                                                        |
| rectangle_set_height     | void                          | struct GameObject_Rectangle *rectangle, int newHeight                                         | Sets the height of the rectangle.                                                           |
| rectangle_get_character  | char                          | struct GameObject_Rectangle *rectangle                                                        | Returns the display character of the rectangle.                                             |
| rectangle_set_character  | void                          | struct GameObject_Rectangle *rectangle, char newCharacter                                     | Sets the display character of the rectangle.                                                |
| rectangle_enable_filled  | void                          | struct GameObject_Rectangle *rectangle                                                        | Enables filled mode (`filled` = 1).                                                         |
| rectangle_disable_filled | void                          | struct GameObject_Rectangle *rectangle                                                        | Disables filled mode (`filled` = 0).                                                        |
| rectangle_get_points     | struct GameObject_Point*      | struct GameObject_Rectangle *rectangle                                                        | Allocates and returns an array of all `GameObject_Point` covering the rectangle.            |
