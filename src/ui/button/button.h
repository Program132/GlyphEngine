#ifndef BUTTON_H
#define BUTTON_H

#include "../../vec2/vec2.h"
#include "../../color/color.h"

struct UIButton;

typedef void (*ButtonCallback)(struct UIButton *button, void *user_data);

struct UIButton {
    struct Vector2 position;
    int width;
    int height;
    char label[64];
    Color normal_fg;
    Color normal_bg;
    Color focused_fg;
    Color focused_bg;
    int is_focused;
    ButtonCallback on_click;
    void *user_data;
};

struct UIButton* button_new(int x, int y, int width, int height, const char *label);
struct UIButton* button_new_full(int x, int y, int width, int height, const char *label,
                                Color normal_fg, Color normal_bg,
                                Color focused_fg, Color focused_bg,
                                ButtonCallback on_click, void *user_data);
void button_build(struct UIButton *button, int x, int y, int width, int height, const char *label);
void button_free(struct UIButton *button);

void button_set_position(struct UIButton *button, int x, int y);
struct Vector2 button_get_position(struct UIButton *button);
void button_set_size(struct UIButton *button, int width, int height);
void button_set_label(struct UIButton *button, const char *label);
const char* button_get_label(struct UIButton *button);

void button_set_colors(struct UIButton *button, Color normal_fg, Color normal_bg, Color focused_fg, Color focused_bg);
void button_set_focused(struct UIButton *button, int focused);
int button_is_focused(struct UIButton *button);

void button_set_on_click(struct UIButton *button, ButtonCallback callback, void *user_data);
void button_trigger(struct UIButton *button);

#endif
