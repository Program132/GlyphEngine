#include "panel.h"
#include <stdlib.h>
#include <string.h>

struct UIPanel* panel_new(int x, int y, int width, int height, const char *title) {
    return panel_new_styled(x, y, width, height, title, '#', ' ', COLOR_WHITE, COLOR_DEFAULT);
}

struct UIPanel* panel_new_styled(int x, int y, int width, int height, const char *title,
                                 char border_char, char fill_char, Color fg, Color bg) {
    struct UIPanel *panel = (struct UIPanel*)malloc(sizeof(struct UIPanel));
    if (panel == NULL) return NULL;
    panel->position.x = x;
    panel->position.y = y;
    panel->width = width > 0 ? width : 1;
    panel->height = height > 0 ? height : 1;
    panel_set_title(panel, title);
    panel->border_char = border_char;
    panel->fill_char = fill_char;
    panel->fg = fg;
    panel->bg = bg;
    return panel;
}

void panel_build(struct UIPanel *panel, int x, int y, int width, int height, const char *title) {
    if (panel == NULL) return;
    panel->position.x = x;
    panel->position.y = y;
    panel->width = width > 0 ? width : 1;
    panel->height = height > 0 ? height : 1;
    panel_set_title(panel, title);
    panel->border_char = '#';
    panel->fill_char = ' ';
    panel->fg = COLOR_WHITE;
    panel->bg = COLOR_DEFAULT;
}

void panel_free(struct UIPanel *panel) {
    if (panel != NULL) {
        free(panel);
    }
}

void panel_set_position(struct UIPanel *panel, int x, int y) {
    if (panel == NULL) return;
    panel->position.x = x;
    panel->position.y = y;
}

struct Vector2 panel_get_position(struct UIPanel *panel) {
    if (panel == NULL) {
        struct Vector2 zero = {0, 0};
        return zero;
    }
    return panel->position;
}

void panel_set_size(struct UIPanel *panel, int width, int height) {
    if (panel == NULL) return;
    panel->width = width > 0 ? width : 1;
    panel->height = height > 0 ? height : 1;
}

void panel_set_title(struct UIPanel *panel, const char *title) {
    if (panel == NULL) return;
    if (title == NULL) {
        panel->title[0] = '\0';
    } else {
        strncpy(panel->title, title, sizeof(panel->title) - 1);
        panel->title[sizeof(panel->title) - 1] = '\0';
    }
}

const char* panel_get_title(struct UIPanel *panel) {
    if (panel == NULL) return "";
    return panel->title;
}

void panel_set_style(struct UIPanel *panel, char border_char, char fill_char, Color fg, Color bg) {
    if (panel == NULL) return;
    panel->border_char = border_char;
    panel->fill_char = fill_char;
    panel->fg = fg;
    panel->bg = bg;
}
