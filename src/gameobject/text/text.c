#include "text.h"
#include <stdlib.h>
#include <string.h>

struct GameObject_Text* text_new(int x, int y, const char *content) {
    return text_new_colored(x, y, content, COLOR_DEFAULT, COLOR_DEFAULT);
}

struct GameObject_Text* text_new_colored(int x, int y, const char *content, Color fg, Color bg) {
    struct GameObject_Text *text_obj = (struct GameObject_Text*)malloc(sizeof(struct GameObject_Text));
    if (text_obj == NULL) return NULL;
    text_build(text_obj, x, y, content, fg, bg);
    return text_obj;
}

void text_build(struct GameObject_Text *text_obj, int x, int y, const char *content, Color fg, Color bg) {
    if (text_obj == NULL) return;
    text_obj->position.x = x;
    text_obj->position.y = y;
    text_obj->fg = fg;
    text_obj->bg = bg;
    text_set_content(text_obj, content);
}

void text_free(struct GameObject_Text *text_obj) {
    if (text_obj != NULL) {
        free(text_obj);
    }
}

void text_set_content(struct GameObject_Text *text_obj, const char *content) {
    if (text_obj == NULL) return;
    if (content == NULL) {
        text_obj->text[0] = '\0';
    } else {
        strncpy(text_obj->text, content, sizeof(text_obj->text) - 1);
        text_obj->text[sizeof(text_obj->text) - 1] = '\0';
    }
}

const char* text_get_content(struct GameObject_Text *text_obj) {
    if (text_obj == NULL) return "";
    return text_obj->text;
}

void text_set_position(struct GameObject_Text *text_obj, int x, int y) {
    if (text_obj == NULL) return;
    text_obj->position.x = x;
    text_obj->position.y = y;
}

struct Vector2 text_get_position(struct GameObject_Text *text_obj) {
    if (text_obj == NULL) {
        struct Vector2 zero = {0, 0};
        return zero;
    }
    return text_obj->position;
}

void text_set_color(struct GameObject_Text *text_obj, Color fg, Color bg) {
    if (text_obj == NULL) return;
    text_obj->fg = fg;
    text_obj->bg = bg;
}

Color text_get_fg(struct GameObject_Text *text_obj) {
    if (text_obj == NULL) return COLOR_DEFAULT;
    return text_obj->fg;
}

Color text_get_bg(struct GameObject_Text *text_obj) {
    if (text_obj == NULL) return COLOR_DEFAULT;
    return text_obj->bg;
}
