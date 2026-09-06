# LevelUI Module

`LevelUI` is a specialized scene type for menus, dialogs, inventories, and user interfaces in GlyphEngine. It provides panels with borders (`UIPanel`), interactive focusable buttons (`UIButton`), and text labels (`GameObject_Text`), with built-in keyboard navigation.

---

## UIPanel

Rectangular panel used for grouping UI components or styling window containers.

### Struct

| Field       | Type           | Description                                   |
|-------------|----------------|-----------------------------------------------|
| position    | struct Vector2 | Top-left position (x, y)                      |
| width       | int            | Panel width in columns                        |
| height      | int            | Panel height in rows                          |
| title       | char[64]       | Optional title rendered on top border         |
| border_char | char           | Character used for borders (e.g., `#` or `+`) |
| fill_char   | char           | Character used for interior (e.g., `' '`)     |
| fg          | Color          | Foreground ANSI color                         |
| bg          | Color          | Background ANSI color                         |

### Functions

| Name               | Type             | Arguments                                                                                                  | Description                                                              |
|--------------------|------------------|------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------|
| panel_new          | struct UIPanel*  | int x, int y, int width, int height, const char *title                                                     | Creates a panel with default border `#` and fill `' '`.                  |
| panel_new_styled   | struct UIPanel*  | int x, int y, int width, int height, const char *title, char border_char, char fill_char, Color fg, Color bg | Creates a panel with custom border, fill character, and ANSI colors.     |
| panel_build        | void             | struct UIPanel *panel, int x, int y, int width, int height, const char *title                              | Initializes an existing panel instance.                                  |
| panel_free         | void             | struct UIPanel *panel                                                                                      | Frees dynamically allocated memory for the panel.                        |
| panel_set_position | void             | struct UIPanel *panel, int x, int y                                                                        | Updates top-left position.                                               |
| panel_set_size     | void             | struct UIPanel *panel, int width, int height                                                               | Updates width and height.                                                |
| panel_set_title    | void             | struct UIPanel *panel, const char *title                                                                   | Updates panel title.                                                     |
| panel_set_style    | void             | struct UIPanel *panel, char border_char, char fill_char, Color fg, Color bg                                | Updates border/fill characters and colors.                              |

---

## UIButton

Interactive button supporting normal and focused states, with a click callback.

### Struct

| Field       | Type           | Description                                       |
|-------------|----------------|---------------------------------------------------|
| position    | struct Vector2 | Top-left position (x, y)                          |
| width       | int            | Button width                                      |
| height      | int            | Button height                                     |
| label       | char[64]       | Centered button label text                        |
| normal_fg   | Color          | Foreground color when unfocused                   |
| normal_bg   | Color          | Background color when unfocused                   |
| focused_fg  | Color          | Foreground color when focused/selected            |
| focused_bg  | Color          | Background color when focused/selected            |
| is_focused  | int            | 1 if button is currently focused, 0 otherwise     |
| on_click    | ButtonCallback | Function pointer: `void (*)(UIButton*, void*)`    |
| user_data   | void*          | Custom user data passed to `on_click`             |

### Functions

| Name              | Type              | Arguments                                                                                                                        | Description                                                          |
|-------------------|-------------------|----------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------|
| button_new        | struct UIButton*  | int x, int y, int width, int height, const char *label                                                                           | Creates a button with default styling.                               |
| button_new_full   | struct UIButton*  | int x, int y, int width, int height, const char *label, Color normal_fg, Color normal_bg, Color focused_fg, Color focused_bg, ...| Creates a button with custom colors and callback.                    |
| button_free       | void              | struct UIButton *button                                                                                                          | Frees dynamically allocated memory for the button.                   |
| button_set_colors | void              | struct UIButton *button, Color normal_fg, Color normal_bg, Color focused_fg, Color focused_bg                                    | Sets normal and focused color schemes.                               |
| button_set_focused| void              | struct UIButton *button, int focused                                                                                             | Sets button focus state (1 = focused, 0 = unfocused).                |
| button_is_focused | int               | struct UIButton *button                                                                                                          | Returns 1 if button is focused, 0 otherwise.                         |
| button_set_on_click| void             | struct UIButton *button, ButtonCallback callback, void *user_data                                                                | Assigns click event handler.                                         |
| button_trigger    | void              | struct UIButton *button                                                                                                          | Manually invokes the button's `on_click` callback.                   |

---

## LevelUI

Container scene managing panels, buttons, and text labels with input navigation.

### Struct

| Field                | Type                       | Description                                      |
|----------------------|----------------------------|--------------------------------------------------|
| name                 | char*                      | Scene title                                      |
| sizeX                | int                        | UI width in columns                              |
| sizeY                | int                        | UI height in rows                                |
| default_char         | char                       | Background fill character                        |
| default_fg           | Color                      | Default background foreground color              |
| default_bg           | Color                      | Default background color                         |
| panels               | struct UIPanel*[64]        | Array of contained panels                        |
| buttons              | struct UIButton*[64]       | Array of contained buttons                       |
| texts                | struct GameObject_Text*[64]| Array of contained text labels                   |
| focused_button_index | int                        | Index of currently focused button (-1 if none)   |

### Functions

| Name                      | Type               | Arguments                                                       | Description                                                                  |
|---------------------------|--------------------|-----------------------------------------------------------------|------------------------------------------------------------------------------|
| level_ui_new              | struct LevelUI*    | const char *name, int sizeX, int sizeY, char default_char       | Allocates and initializes a new UI level.                                    |
| level_ui_free             | void               | struct LevelUI *ui                                              | Frees the UI level and all contained panels, buttons, and text objects.      |
| level_ui_set_default_color| void               | struct LevelUI *ui, Color fg, Color bg                          | Sets background colors for empty cells.                                      |
| level_ui_add_panel        | void               | struct LevelUI *ui, struct UIPanel *panel                       | Adds a panel to the UI scene.                                               |
| level_ui_add_button       | void               | struct LevelUI *ui, struct UIButton *button                     | Adds a button to the UI scene (auto-focuses the first button).               |
| level_ui_add_text         | void               | struct LevelUI *ui, struct GameObject_Text *text                | Adds a text label to the UI scene.                                           |
| level_ui_navigate_next    | void               | struct LevelUI *ui                                              | Cycles focus to the next button.                                             |
| level_ui_navigate_prev    | void               | struct LevelUI *ui                                              | Cycles focus to the previous button.                                         |
| level_ui_set_focused_button| void              | struct LevelUI *ui, int index                                   | Focuses button at specified index.                                           |
| level_ui_get_focused_button| struct UIButton*  | struct LevelUI *ui                                              | Returns pointer to the currently focused button.                             |
| level_ui_click_focused    | void               | struct LevelUI *ui                                              | Triggers click event on the currently focused button.                        |
| level_ui_handle_input     | void               | struct LevelUI *ui                                              | Automatically handles UP/DOWN/TAB (navigation) and ENTER/SPACE (click).      |
| level_ui_display          | void               | struct LevelUI *ui                                              | Renders the UI scene with double buffering and optimized ANSI color output. |
| level_ui_to_level         | struct Level*      | struct LevelUI *ui                                              | Converts UI elements into a standard `Level*` scene.                         |
