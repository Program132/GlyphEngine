#include "button.h"
#include <stdlib.h>
#include <string.h>

struct UIButton* button_new(int x, int y, int width, int height, const char *label) {
    return button_new_full(x, y, width, height, label,
                           COLOR_WHITE, COLOR_BRIGHT_BLACK,
                           COLOR_BLACK, COLOR_BRIGHT_YELLOW,
                           NULL, NULL);
}

struct UIButton* button_new_full(int x, int y, int width, int height, const char *label,
                                Color normal_fg, Color normal_bg,
                                Color focused_fg, Color focused_bg,
                                ButtonCallback on_click, void *user_data) {
    struct UIButton *button = (struct UIButton*)malloc(sizeof(struct UIButton));
    if (button == NULL) return NULL;
    button->position.x = x;
    button->position.y = y;
    button->width = width > 0 ? width : 1;
    button->height = height > 0 ? height : 1;
    button_set_label(button, label);
    button->normal_fg = normal_fg;
    button->normal_bg = normal_bg;
    button->focused_fg = focused_fg;
    button->focused_bg = focused_bg;
    button->is_focused = 0;
    button->on_click = on_click;
    button->user_data = user_data;
    return button;
}

void button_build(struct UIButton *button, int x, int y, int width, int height, const char *label) {
    if (button == NULL) return;
    button->position.x = x;
    button->position.y = y;
    button->width = width > 0 ? width : 1;
    button->height = height > 0 ? height : 1;
    button_set_label(button, label);
    button->normal_fg = COLOR_WHITE;
    button->normal_bg = COLOR_BRIGHT_BLACK;
    button->focused_fg = COLOR_BLACK;
    button->focused_bg = COLOR_BRIGHT_YELLOW;
    button->is_focused = 0;
    button->on_click = NULL;
    button->user_data = NULL;
}

void button_free(struct UIButton *button) {
    if (button != NULL) {
        free(button);
    }
}

void button_set_position(struct UIButton *button, int x, int y) {
    if (button == NULL) return;
    button->position.x = x;
    button->position.y = y;
}

struct Vector2 button_get_position(struct UIButton *button) {
    if (button == NULL) {
        struct Vector2 zero = {0, 0};
        return zero;
    }
    return button->position;
}

void button_set_size(struct UIButton *button, int width, int height) {
    if (button == NULL) return;
    button->width = width > 0 ? width : 1;
    button->height = height > 0 ? height : 1;
}

void button_set_label(struct UIButton *button, const char *label) {
    if (button == NULL) return;
    if (label == NULL) {
        button->label[0] = '\0';
    } else {
        strncpy(button->label, label, sizeof(button->label) - 1);
        button->label[sizeof(button->label) - 1] = '\0';
    }
}

const char* button_get_label(struct UIButton *button) {
    if (button == NULL) return "";
    return button->label;
}

void button_set_colors(struct UIButton *button, Color normal_fg, Color normal_bg, Color focused_fg, Color focused_bg) {
    if (button == NULL) return;
    button->normal_fg = normal_fg;
    button->normal_bg = normal_bg;
    button->focused_fg = focused_fg;
    button->focused_bg = focused_bg;
}

void button_set_focused(struct UIButton *button, int focused) {
    if (button == NULL) return;
    button->is_focused = focused;
}

int button_is_focused(struct UIButton *button) {
    if (button == NULL) return 0;
    return button->is_focused;
}

void button_set_on_click(struct UIButton *button, ButtonCallback callback, void *user_data) {
    if (button == NULL) return;
    button->on_click = callback;
    button->user_data = user_data;
}

void button_trigger(struct UIButton *button) {
    if (button == NULL) return;
    if (button->on_click != NULL) {
        button->on_click(button, button->user_data);
    }
}
