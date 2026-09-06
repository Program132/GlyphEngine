# Texture Module

The `Texture` module provides 2D ASCII art patterns and sprites for game objects. Textures support transparency through a designated `transparent_char` (allowing background elements to show through empty spaces), making it easy to create detailed multi-character player sprites, tiled walls, platforms, and props.

# Struct

| Argument         | Type   | Description                                                                     |
|------------------|--------|---------------------------------------------------------------------------------|
| width            | int    | Width of the texture in characters.                                             |
| height           | int    | Height of the texture in characters.                                            |
| pixels           | char*  | Flat buffer of `width * height` characters.                                     |
| transparent_char | char   | Character considered transparent (e.g. `' '`), allowing background to display. |

# Functions

| Name              | Type            | Arguments                                                        | Description                                                                                          |
|-------------------|-----------------|------------------------------------------------------------------|------------------------------------------------------------------------------------------------------|
| texture_new       | struct Texture* | int width, int height, const char *data, char transparent_char   | Dynamically allocates a new texture from a pattern string, automatically filtering newlines.         |
| texture_build     | void            | struct Texture *tex, int w, int h, const char *d, char trans     | Initializes the fields and allocates pixels for an existing Texture instance.                         |
| texture_free      | void            | struct Texture *texture                                          | Frees dynamically allocated memory for the texture and its pixel buffer.                            |
| texture_get_pixel | char            | struct Texture *texture, int x, int y                            | Returns the character at (x, y) or `transparent_char` if out of bounds.                             |
| texture_set_pixel | void            | struct Texture *texture, int x, int y, char c                    | Sets the character at (x, y) if within bounds.                                                       |

# Usage Example

```c
#include "texture/texture.h"

// Define a 3x3 spaceship sprite with space (' ') as the transparent character
const char *ship_art = 
    " ^ \n"
    "/#\\\n"
    "d b";

struct Texture *ship_tex = texture_new(3, 3, ship_art, ' ');
```
