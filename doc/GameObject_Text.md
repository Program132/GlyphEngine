# Struct

| Argument  | Type            | Description                                        |
|-----------|-----------------|----------------------------------------------------|
| position  | struct Vector2  | 2D position (top-left) of the text string          |
| text      | char[256]       | String content rendered in the level               |
| fg        | Color           | Foreground ANSI color                              |
| bg        | Color           | Background ANSI color                              |

# Functions

| Name              | Type                     | Arguments                                                                   | Description                                                                  |
|-------------------|--------------------------|-----------------------------------------------------------------------------|------------------------------------------------------------------------------|
| text_new          | struct GameObject_Text*  | int x, int y, const char *content                                           | Dynamically allocates and initializes a new text object at (x, y).           |
| text_new_colored  | struct GameObject_Text*  | int x, int y, const char *content, Color fg, Color bg                       | Allocates and initializes a text object with custom foreground & background. |
| text_build        | void                     | struct GameObject_Text *text_obj, int x, int y, const char *content, Color fg, Color bg | Initializes fields of an existing GameObject_Text instance.                  |
| text_free         | void                     | struct GameObject_Text *text_obj                                            | Frees dynamically allocated memory for the text object.                      |
| text_set_content  | void                     | struct GameObject_Text *text_obj, const char *content                       | Updates the text content.                                                    |
| text_get_content  | const char*              | struct GameObject_Text *text_obj                                            | Returns the current text string.                                             |
| text_set_position | void                     | struct GameObject_Text *text_obj, int x, int y                              | Sets the top-left position of the text object.                               |
| text_get_position | struct Vector2           | struct GameObject_Text *text_obj                                            | Returns the current position (Vector2) of the text object.                  |
| text_set_color    | void                     | struct GameObject_Text *text_obj, Color fg, Color bg                        | Sets the foreground and background ANSI colors.                              |
| text_get_fg       | Color                    | struct GameObject_Text *text_obj                                            | Returns the foreground color.                                                |
| text_get_bg       | Color                    | struct GameObject_Text *text_obj                                            | Returns the background color.                                                |
