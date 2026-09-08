#include "levelui.h"
#include "../inputs/inputs.h"
#include "../utils/utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct ScreenCell {
    char ch;
    Color fg;
    Color bg;
};

static struct ScreenCell *s_ui_prev_cells = NULL;
static int s_ui_prev_rows = 0;
static int s_ui_prev_cols = 0;

void level_ui_reset_screen_buffer(void) {
    if (s_ui_prev_cells != NULL) {
        free(s_ui_prev_cells);
        s_ui_prev_cells = NULL;
    }
    s_ui_prev_rows = 0;
    s_ui_prev_cols = 0;
}

struct LevelUI* level_ui_new(const char *name, int sizeX, int sizeY, char default_char) {
    struct LevelUI *ui = (struct LevelUI*)malloc(sizeof(struct LevelUI));
    if (ui == NULL) return NULL;

    if (name != NULL) {
        ui->name = (char*)malloc(strlen(name) + 1);
        if (ui->name != NULL) strcpy(ui->name, name);
    } else {
        ui->name = NULL;
    }

    ui->sizeX = sizeX;
    ui->sizeY = sizeY;
    ui->default_char = default_char;
    ui->default_fg = COLOR_DEFAULT;
    ui->default_bg = COLOR_DEFAULT;
    ui->panel_count = 0;
    ui->button_count = 0;
    ui->text_count = 0;
    ui->focused_button_index = -1;

    for (int i = 0; i < MAX_UI_ELEMENTS; i++) {
        ui->panels[i] = NULL;
        ui->buttons[i] = NULL;
        ui->texts[i] = NULL;
    }

    return ui;
}

void level_ui_free(struct LevelUI *ui) {
    if (ui == NULL) return;

    if (ui->name != NULL) free(ui->name);

    for (int i = 0; i < ui->panel_count; i++) {
        if (ui->panels[i] != NULL) panel_free(ui->panels[i]);
    }

    for (int i = 0; i < ui->button_count; i++) {
        if (ui->buttons[i] != NULL) button_free(ui->buttons[i]);
    }

    for (int i = 0; i < ui->text_count; i++) {
        if (ui->texts[i] != NULL) text_free(ui->texts[i]);
    }

    free(ui);
}

void level_ui_set_default_color(struct LevelUI *ui, Color fg, Color bg) {
    if (ui == NULL) return;
    ui->default_fg = fg;
    ui->default_bg = bg;
}

void level_ui_add_panel(struct LevelUI *ui, struct UIPanel *panel) {
    if (ui == NULL || panel == NULL) return;
    if (ui->panel_count < MAX_UI_ELEMENTS) {
        ui->panels[ui->panel_count++] = panel;
    }
}

void level_ui_add_button(struct LevelUI *ui, struct UIButton *button) {
    if (ui == NULL || button == NULL) return;
    if (ui->button_count < MAX_UI_ELEMENTS) {
        ui->buttons[ui->button_count] = button;
        if (ui->button_count == 0) {
            ui->focused_button_index = 0;
            button_set_focused(button, 1);
        } else {
            button_set_focused(button, 0);
        }
        ui->button_count++;
    }
}

void level_ui_add_text(struct LevelUI *ui, struct GameObject_Text *text) {
    if (ui == NULL || text == NULL) return;
    if (ui->text_count < MAX_UI_ELEMENTS) {
        ui->texts[ui->text_count++] = text;
    }
}

void level_ui_navigate_next(struct LevelUI *ui) {
    if (ui == NULL || ui->button_count == 0) return;
    if (ui->focused_button_index >= 0 && ui->focused_button_index < ui->button_count) {
        button_set_focused(ui->buttons[ui->focused_button_index], 0);
    }
    ui->focused_button_index = (ui->focused_button_index + 1) % ui->button_count;
    button_set_focused(ui->buttons[ui->focused_button_index], 1);
}

void level_ui_navigate_prev(struct LevelUI *ui) {
    if (ui == NULL || ui->button_count == 0) return;
    if (ui->focused_button_index >= 0 && ui->focused_button_index < ui->button_count) {
        button_set_focused(ui->buttons[ui->focused_button_index], 0);
    }
    ui->focused_button_index = (ui->focused_button_index - 1 + ui->button_count) % ui->button_count;
    button_set_focused(ui->buttons[ui->focused_button_index], 1);
}

void level_ui_set_focused_button(struct LevelUI *ui, int index) {
    if (ui == NULL || ui->button_count == 0) return;
    if (index < 0 || index >= ui->button_count) return;
    if (ui->focused_button_index >= 0 && ui->focused_button_index < ui->button_count) {
        button_set_focused(ui->buttons[ui->focused_button_index], 0);
    }
    ui->focused_button_index = index;
    button_set_focused(ui->buttons[ui->focused_button_index], 1);
}

struct UIButton* level_ui_get_focused_button(struct LevelUI *ui) {
    if (ui == NULL || ui->focused_button_index < 0 || ui->focused_button_index >= ui->button_count) {
        return NULL;
    }
    return ui->buttons[ui->focused_button_index];
}

void level_ui_click_focused(struct LevelUI *ui) {
    struct UIButton *btn = level_ui_get_focused_button(ui);
    if (btn != NULL) {
        button_trigger(btn);
    }
}

void level_ui_handle_input(struct LevelUI *ui) {
    if (ui == NULL) return;

    static int prev_up = 1;
    static int prev_down = 1;
    static int prev_action = 1;

    int up = input_is_key_down(KEY_UP) || input_is_key_down('w') || input_is_key_down('W') || input_is_key_down('z') || input_is_key_down('Z');
    int down = input_is_key_down(KEY_DOWN) || input_is_key_down('s') || input_is_key_down('S');
    int action = input_is_key_down(KEY_ENTER) || input_is_key_down(KEY_RETURN) || input_is_key_down(KEY_SPACE);

    if (up && !prev_up) {
        level_ui_navigate_prev(ui);
    }
    if (down && !prev_down) {
        level_ui_navigate_next(ui);
    }
    if (action && !prev_action) {
        level_ui_click_focused(ui);
    }

    prev_up = up;
    prev_down = down;
    prev_action = action;
}

void level_ui_display(struct LevelUI *ui) {
    if (ui == NULL) return;

    int total_rows = ui->sizeY;
    int total_cols = ui->sizeX;

    struct ScreenCell *curr_cells = (struct ScreenCell*)malloc(sizeof(struct ScreenCell) * total_rows * total_cols);
    if (curr_cells == NULL) return;

    for (int y = 0; y < total_rows; y++) {
        for (int x = 0; x < total_cols; x++) {
            char toPrint = ui->default_char;
            Color cell_fg = ui->default_fg;
            Color cell_bg = ui->default_bg;

            for (int p = 0; p < ui->panel_count; p++) {
                struct UIPanel *panel = ui->panels[p];
                if (panel == NULL) continue;
                if (x >= panel->position.x && x < panel->position.x + panel->width &&
                    y >= panel->position.y && y < panel->position.y + panel->height) {

                    int is_border = (x == panel->position.x || x == panel->position.x + panel->width - 1 ||
                                     y == panel->position.y || y == panel->position.y + panel->height - 1);

                    if (is_border) {
                        int tlen = (int)strlen(panel->title);
                        if (y == panel->position.y && tlen > 0 &&
                            x >= panel->position.x + 2 && x < panel->position.x + 2 + tlen &&
                            panel->position.x + 2 + tlen < panel->position.x + panel->width - 1) {
                            toPrint = panel->title[x - (panel->position.x + 2)];
                        } else {
                            toPrint = panel->border_char;
                        }
                    } else {
                        toPrint = panel->fill_char;
                    }
                    cell_fg = panel->fg;
                    cell_bg = panel->bg;
                }
            }

            for (int b = 0; b < ui->button_count; b++) {
                struct UIButton *btn = ui->buttons[b];
                if (btn == NULL) continue;
                if (x >= btn->position.x && x < btn->position.x + btn->width &&
                    y >= btn->position.y && y < btn->position.y + btn->height) {

                    cell_fg = btn->is_focused ? btn->focused_fg : btn->normal_fg;
                    cell_bg = btn->is_focused ? btn->focused_bg : btn->normal_bg;

                    int blen = (int)strlen(btn->label);
                    int off_x = (btn->width - blen) / 2;
                    int off_y = btn->height / 2;

                    if (y == btn->position.y + off_y &&
                        x >= btn->position.x + off_x && x < btn->position.x + off_x + blen) {
                        toPrint = btn->label[x - (btn->position.x + off_x)];
                    } else if (btn->height > 2 && (x == btn->position.x || x == btn->position.x + btn->width - 1 ||
                                                  y == btn->position.y || y == btn->position.y + btn->height - 1)) {
                        toPrint = btn->is_focused ? '=' : '-';
                    } else {
                        toPrint = ' ';
                    }
                }
            }

            for (int t = 0; t < ui->text_count; t++) {
                struct GameObject_Text *txt = ui->texts[t];
                if (txt == NULL || txt->text[0] == '\0') continue;
                if (y == txt->position.y) {
                    int len = (int)strlen(txt->text);
                    if (x >= txt->position.x && x < txt->position.x + len) {
                        toPrint = txt->text[x - txt->position.x];
                        cell_fg = txt->fg;
                        cell_bg = txt->bg;
                    }
                }
            }

            curr_cells[y * total_cols + x] = (struct ScreenCell){toPrint, cell_fg, cell_bg};
        }
    }

    int buf_cap = total_rows * (total_cols * 32 + 32) + 4096;
    char *out_buf = (char*)malloc(buf_cap);
    if (out_buf == NULL) {
        free(curr_cells);
        return;
    }
    int out_idx = 0;

    int need_full_redraw = (s_ui_prev_cells == NULL || s_ui_prev_rows != total_rows || s_ui_prev_cols != total_cols);

    Color active_fg = COLOR_DEFAULT;
    Color active_bg = COLOR_DEFAULT;

    if (need_full_redraw) {
        clearConsoleScreen();
        for (int r = 0; r < total_rows; r++) {
            for (int c = 0; c < total_cols; c++) {
                struct ScreenCell *cell = &curr_cells[r * total_cols + c];
                if (cell->fg != active_fg) {
                    const char *fg_ansi = color_to_ansi_fg(cell->fg);
                    while (*fg_ansi) out_buf[out_idx++] = *fg_ansi++;
                    active_fg = cell->fg;
                }
                if (cell->bg != active_bg) {
                    const char *bg_ansi = color_to_ansi_bg(cell->bg);
                    while (*bg_ansi) out_buf[out_idx++] = *bg_ansi++;
                    active_bg = cell->bg;
                }
                out_buf[out_idx++] = cell->ch;
            }
            if (active_fg != COLOR_DEFAULT || active_bg != COLOR_DEFAULT) {
                const char *rst = color_reset_ansi();
                while (*rst) out_buf[out_idx++] = *rst++;
                active_fg = COLOR_DEFAULT;
                active_bg = COLOR_DEFAULT;
            }
            if (r < total_rows - 1) {
                out_buf[out_idx++] = '\n';
            }
        }
    } else {
        for (int r = 0; r < total_rows; r++) {
            int c = 0;
            while (c < total_cols) {
                int idx = r * total_cols + c;
                if (curr_cells[idx].ch != s_ui_prev_cells[idx].ch ||
                    curr_cells[idx].fg != s_ui_prev_cells[idx].fg ||
                    curr_cells[idx].bg != s_ui_prev_cells[idx].bg) {

                    out_idx += snprintf(&out_buf[out_idx], buf_cap - out_idx, "\033[%d;%dH", r + 1, c + 1);

                    while (c < total_cols) {
                        int cidx = r * total_cols + c;
                        if (curr_cells[cidx].ch == s_ui_prev_cells[cidx].ch &&
                            curr_cells[cidx].fg == s_ui_prev_cells[cidx].fg &&
                            curr_cells[cidx].bg == s_ui_prev_cells[cidx].bg) {
                            break;
                        }
                        struct ScreenCell *cell = &curr_cells[cidx];
                        if (cell->fg != active_fg) {
                            const char *fg_ansi = color_to_ansi_fg(cell->fg);
                            while (*fg_ansi) out_buf[out_idx++] = *fg_ansi++;
                            active_fg = cell->fg;
                        }
                        if (cell->bg != active_bg) {
                            const char *bg_ansi = color_to_ansi_bg(cell->bg);
                            while (*bg_ansi) out_buf[out_idx++] = *bg_ansi++;
                            active_bg = cell->bg;
                        }
                        out_buf[out_idx++] = cell->ch;
                        c++;
                    }
                } else {
                    c++;
                }
            }
        }
    }

    if (active_fg != COLOR_DEFAULT || active_bg != COLOR_DEFAULT) {
        const char *rst = color_reset_ansi();
        while (*rst) out_buf[out_idx++] = *rst++;
    }
    out_buf[out_idx] = '\0';

    if (out_idx > 0) {
        fputs(out_buf, stdout);
        fflush(stdout);
    }

    free(out_buf);

    if (s_ui_prev_cells == NULL || s_ui_prev_rows != total_rows || s_ui_prev_cols != total_cols) {
        if (s_ui_prev_cells != NULL) free(s_ui_prev_cells);
        s_ui_prev_cells = (struct ScreenCell*)malloc(sizeof(struct ScreenCell) * total_rows * total_cols);
        s_ui_prev_rows = total_rows;
        s_ui_prev_cols = total_cols;
    }

    if (s_ui_prev_cells != NULL) {
        memcpy(s_ui_prev_cells, curr_cells, sizeof(struct ScreenCell) * total_rows * total_cols);
    }

    free(curr_cells);
}

struct Level* level_ui_to_level(struct LevelUI *ui) {
    if (ui == NULL) return NULL;
    struct Level *level = level_new(ui->name != NULL ? ui->name : "UI", ui->sizeX, ui->sizeY, ui->default_char);
    if (level == NULL) return NULL;
    level_set_default_color(level, ui->default_fg, ui->default_bg);

    for (int p = 0; p < ui->panel_count; p++) {
        struct UIPanel *pan = ui->panels[p];
        if (pan == NULL) continue;
        struct GameObject_Rectangle *rect = rectangle_new(pan->position.x, pan->position.y, pan->width, pan->height, pan->border_char);
        if (rect != NULL) {
            rectangle_set_color(rect, pan->fg, pan->bg);
            level_add_rectangle(level, rect);
        }
    }

    for (int t = 0; t < ui->text_count; t++) {
        struct GameObject_Text *txt = ui->texts[t];
        if (txt == NULL) continue;
        struct GameObject_Text *copy = text_new_colored(txt->position.x, txt->position.y, txt->text, txt->fg, txt->bg);
        if (copy != NULL) {
            level_add_text(level, copy);
        }
    }

    return level;
}
