# GameObject_Circle

The "Circle" object is not a distinct geometric type in `Level`. It is a convenience constructor that creates a `GameObject_Ellipse` whose width and height are equal (derived from the specified diameter), simplifying boilerplate code.

# Functions

| Name                | Type                       | Arguments                                                                     | Description                                                                              |
|---------------------|----------------------------|-------------------------------------------------------------------------------|------------------------------------------------------------------------------------------|
| circle_new          | struct GameObject_Ellipse* | int x, int y, int diameter, char character                                    | Dynamically allocates and initializes a new ellipse with equal axes (circle).           |
| circle_new_textured | struct GameObject_Ellipse* | int x, int y, int diameter, struct Texture *texture                           | Dynamically allocates and initializes a new textured circle.                             |
| circle_build        | void                       | struct GameObject_Ellipse *circle, int x, int y, int diameter, char character | Initializes the fields of an existing GameObject_Ellipse instance as a circle.          |

Once created, the circle can be added to the scene using `level_add_ellipse()`. All standard ellipse methods documented in `GameObject_Ellipses.md` (including `ellipse_set_texture()`) can be used on the returned object.
