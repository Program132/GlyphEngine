# GameObject_Square

The "Square" object is not a distinct geometric type in `Level`. It is a convenience constructor that creates a `GameObject_Rectangle` whose width and height are equal, simplifying boilerplate code.

# Functions

| Name                | Type                         | Arguments                                                                    | Description                                                                              |
|---------------------|------------------------------|------------------------------------------------------------------------------|------------------------------------------------------------------------------------------|
| square_new          | struct GameObject_Rectangle* | int x, int y, int size, char character                                       | Dynamically allocates and initializes a new rectangle with `width == height == size`.   |
| square_new_textured | struct GameObject_Rectangle* | int x, int y, int size, struct Texture *texture                              | Dynamically allocates and initializes a new textured square.                            |
| square_build        | void                         | struct GameObject_Rectangle *square, int x, int y, int size, char character  | Initializes the fields of an existing GameObject_Rectangle instance as a square.         |

Once created, the square can be added to the scene using `level_add_rectangle()`. All standard rectangle methods documented in `GameObject_Rectangles.md` (including `rectangle_set_texture()`) can be used on the returned object.
