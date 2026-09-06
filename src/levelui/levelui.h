#ifndef LEVELUI_H
#define LEVELUI_H

#include "../color/color.h"
#include "../ui/button/button.h"
#include "../ui/panel/panel.h"
#include "../gameobject/text/text.h"
#include "../level/level.h"

#define MAX_UI_ELEMENTS 64

struct LevelUI {
    char *name;
    int sizeX;
    int sizeY;
    char default_char;
    Color default_fg;
    Color default_bg;
    struct UIPanel *panels[MAX_UI_ELEMENTS];
    int panel_count;
    struct UIButton *buttons[MAX_UI_ELEMENTS];
    int button_count;
    struct GameObject_Text *texts[MAX_UI_ELEMENTS];
    int text_count;
    int focused_button_index;
};

struct LevelUI* level_ui_new(const char *name, int sizeX, int sizeY, char default_char);
void level_ui_free(struct LevelUI *ui);
void level_ui_set_default_color(struct LevelUI *ui, Color fg, Color bg);
void level_ui_add_panel(struct LevelUI *ui, struct UIPanel *panel);
void level_ui_add_button(struct LevelUI *ui, struct UIButton *button);
void level_ui_add_text(struct LevelUI *ui, struct GameObject_Text *text);
void level_ui_navigate_next(struct LevelUI *ui);
void level_ui_navigate_prev(struct LevelUI *ui);
void level_ui_set_focused_button(struct LevelUI *ui, int index);
struct UIButton* level_ui_get_focused_button(struct LevelUI *ui);
void level_ui_click_focused(struct LevelUI *ui);
void level_ui_handle_input(struct LevelUI *ui);
void level_ui_display(struct LevelUI *ui);
struct Level* level_ui_to_level(struct LevelUI *ui);

#endif
