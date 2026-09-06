# Color Module

The `Color` module introduces ANSI color styling for all visual elements in GlyphEngine. It supports 16 standard and bright foreground and background terminal colors.

# Types

### `Color` (Enum)

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

# Functions

| Name               | Type         | Arguments   | Description                                                        |
|--------------------|--------------|-------------|--------------------------------------------------------------------|
| color_to_ansi_fg   | const char*  | Color color | Returns the ANSI escape string for the specified foreground color. |
| color_to_ansi_bg   | const char*  | Color color | Returns the ANSI escape string for the specified background color. |
| color_reset_ansi   | const char*  | void        | Returns the ANSI reset escape string (`\033[0m`).                  |

# Usage Example

```c
#include "color/color.h"
#include "gameobject/player/player.h"

// Set green text on default background for the player
player_set_color(player, COLOR_BRIGHT_GREEN, COLOR_DEFAULT);
```
