#ifndef PANEL_H
#define PANEL_H

#include "../../vec2/vec2.h"
#include "../../color/color.h"

struct UIPanel {
    struct Vector2 position;
    int width;
    int height;
    char title[64];
    char border_char;
    char fill_char;
    Color fg;
    Color bg;
};

struct UIPanel* panel_new(int x, int y, int width, int height, const char *title);
struct UIPanel* panel_new_styled(int x, int y, int width, int height, const char *title,
                                 char border_char, char fill_char, Color fg, Color bg);
void panel_build(struct UIPanel *panel, int x, int y, int width, int height, const char *title);
void panel_free(struct UIPanel *panel);

void panel_set_position(struct UIPanel *panel, int x, int y);
struct Vector2 panel_get_position(struct UIPanel *panel);
void panel_set_size(struct UIPanel *panel, int width, int height);
void panel_set_title(struct UIPanel *panel, const char *title);
const char* panel_get_title(struct UIPanel *panel);

void panel_set_style(struct UIPanel *panel, char border_char, char fill_char, Color fg, Color bg);

#endif
