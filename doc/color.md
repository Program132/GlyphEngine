# Color Module

The `Color` module introduces full color styling for all visual elements in GlyphEngine. It supports both classic 16-color ANSI terminal colors and full **24-bit TrueColor RGB (16.7 million colors)** with 100% backward compatibility.

# Types

### `Color` (Enum / 32-bit Integer)

`Color` can store either an ANSI 16 palette constant or a 24-bit TrueColor RGB value.

#### Classic ANSI 16 Colors

| Constant                | Value | Description                      |
|-------------------------|-------|----------------------------------|
| `COLOR_DEFAULT`         | 0     | Terminal default color           |
| `COLOR_BLACK`           | 1     | Standard black                   |
| `COLOR_RED`             | 2     | Standard red                     |
| `COLOR_GREEN`           | 3     | Standard green                   |
| `COLOR_YELLOW`          | 4     | Standard yellow                  |
| `COLOR_BLUE`            | 5     | Standard blue                    |
| `COLOR_MAGENTA`         | 6     | Standard magenta                 |
| `COLOR_CYAN`            | 7     | Standard cyan                    |
| `COLOR_WHITE`           | 8     | Standard white                   |
| `COLOR_BRIGHT_BLACK`    | 9     | Bright black (dark gray)         |
| `COLOR_BRIGHT_RED`      | 10    | Bright red                       |
| `COLOR_BRIGHT_GREEN`    | 11    | Bright green                     |
| `COLOR_BRIGHT_YELLOW`   | 12    | Bright yellow                    |
| `COLOR_BRIGHT_BLUE`     | 13    | Bright blue                      |
| `COLOR_BRIGHT_MAGENTA`  | 14    | Bright magenta                   |
| `COLOR_BRIGHT_CYAN`     | 15    | Bright cyan                      |
| `COLOR_BRIGHT_WHITE`    | 16    | Bright white                     |

#### Predefined TrueColor RGB Constants

| Constant            | Hex Value    | Description                  |
|---------------------|--------------|------------------------------|
| `COLOR_RGB_ORANGE`  | `0xFFA500`   | Vibrant orange               |
| `COLOR_RGB_GOLD`    | `0xFFD700`   | Golden yellow                |
| `COLOR_RGB_PURPLE`  | `0x800080`   | Rich purple                  |
| `COLOR_RGB_CRIMSON` | `0xDC143C`   | Dark crimson red             |
| `COLOR_RGB_LIME`    | `0x00FF00`   | Pure bright lime             |
| `COLOR_RGB_AMBER`   | `0xFFBF00`   | Warm amber                   |
| `COLOR_RGB_TEAL`    | `0x008080`   | Deep teal                    |
| `COLOR_RGB_NAVY`    | `0x000080`   | Dark navy blue               |
| `COLOR_RGB_INDIGO`  | `0x4B0082`   | Deep indigo                  |
| `COLOR_RGB_PINK`    | `0xFFC0CB`   | Soft pink                    |
| `COLOR_RGB_CORAL`   | `0xFF7F50`   | Warm coral                   |
| `COLOR_RGB_BROWN`   | `0x8B4513`   | Saddle brown                 |

# Functions

| Name               | Type          | Arguments                                | Description                                                        |
|--------------------|---------------|------------------------------------------|--------------------------------------------------------------------|
| color_rgb          | Color         | unsigned char r, g, b                    | Creates a 24-bit TrueColor RGB value.                              |
| color_hex          | Color         | unsigned int hex (0xRRGGBB)              | Creates a 24-bit TrueColor from a hexadecimal RGB literal.         |
| color_is_rgb       | int           | Color color                              | Returns 1 if the color is 24-bit TrueColor, 0 if ANSI 16.          |
| color_ansi_to_rgb  | Color         | Color color                              | Converts an ANSI 16 color to its closest RGB equivalent.          |
| color_get_r        | unsigned char | Color color                              | Returns the red component (0-255). Works for ANSI and TrueColor.   |
| color_get_g        | unsigned char | Color color                              | Returns the green component (0-255). Works for ANSI and TrueColor. |
| color_get_b        | unsigned char | Color color                              | Returns the blue component (0-255). Works for ANSI and TrueColor.  |
| color_lerp         | Color         | Color c1, Color c2, float t              | Linearly interpolates between two colors (t from 0.0 to 1.0).      |
| color_brightness   | Color         | Color c, float factor                    | Scales the brightness of a color by factor (e.g. 0.5 = dim).       |
| color_to_ansi_fg   | const char*   | Color color                              | Returns the ANSI foreground sequence (16-color or 24-bit TrueColor).|
| color_to_ansi_bg   | const char*   | Color color                              | Returns the ANSI background sequence (16-color or 24-bit TrueColor).|
| color_reset_ansi   | const char*   | void                                     | Returns the ANSI reset sequence (`\033[0m`).                       |

# Usage Examples

### Classic ANSI Usage
```c
#include "color/color.h"
#include "gameobject/player/player.h"

player_set_color(player, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
```

### 24-bit TrueColor RGB Usage
```c
#include "color/color.h"

Color flame = color_rgb(255, 120, 0);
Color gold = color_hex(0xFFD700);

player_set_color(player, flame, COLOR_DEFAULT);
```

### Color Interpolation (Gradients & Lighting)
```c
Color hot = color_rgb(255, 255, 200);
Color cold = color_rgb(180, 0, 0);

Color mid = color_lerp(hot, cold, 0.5f);
Color dimmed = color_brightness(flame, 0.4f);
```
