#ifndef TEXT_H
#define TEXT_H

#include "../../vec2/vec2.h"
#include "../../color/color.h"

struct GameObject_Text {
    struct Vector2 position;
    char text[256];
    Color fg;
    Color bg;
};

struct GameObject_Text* text_new(int x, int y, const char *content);
struct GameObject_Text* text_new_colored(int x, int y, const char *content, Color fg, Color bg);
void text_build(struct GameObject_Text *text_obj, int x, int y, const char *content, Color fg, Color bg);
void text_free(struct GameObject_Text *text_obj);

void text_set_content(struct GameObject_Text *text_obj, const char *content);
const char* text_get_content(struct GameObject_Text *text_obj);
void text_set_position(struct GameObject_Text *text_obj, int x, int y);
struct Vector2 text_get_position(struct GameObject_Text *text_obj);
void text_set_color(struct GameObject_Text *text_obj, Color fg, Color bg);
Color text_get_fg(struct GameObject_Text *text_obj);
Color text_get_bg(struct GameObject_Text *text_obj);

#endif
