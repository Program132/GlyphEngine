# Struct

| Argument | Type | Description             |
|----------|------|-------------------------|
| x        | int  | Value along the X axis  |
| y        | int  | Value along the Y axis  |

# Functions

| Name       | Type             | Arguments                             | Description                                                                     |
|------------|------------------|---------------------------------------|---------------------------------------------------------------------------------|
| vec2_null  | void             | struct Vector2 *vec2                  | Resets both components of the vector to zero (0, 0).                            |
| vec2_new   | struct Vector2*  | int x, int y                          | Dynamically allocates and initializes a new vector with the given values.       |
| vec2_build | void             | struct Vector2 *vec2, int x, int y    | Initializes the x and y fields of an existing Vector2 instance.                 |
| vec2_setX  | void             | struct Vector2 *vec2, int x           | Sets the value along the X axis.                                                |
| vec2_setY  | void             | struct Vector2 *vec2, int y           | Sets the value along the Y axis.                                                |
| vec2_getX  | int              | struct Vector2 *vec2                  | Returns the value along the X axis.                                             |
| vec2_getY  | int              | struct Vector2 *vec2                  | Returns the value along the Y axis.                                             |
| vec2_str   | char*            | struct Vector2 *vec2                  | Returns a formatted string representation of the vector (e.g. `(x: %d, y: %d)`).|
| vec2_free  | void             | struct Vector2 *vec2                  | Frees dynamically allocated memory for the vector.                              |