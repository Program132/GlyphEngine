#include "level.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/utils.h"

struct Level* level_new(char* levelName, int sizeX, int sizeY, char defaultCharacter) {
    struct Level *level = malloc(sizeof(struct Level));
    if (level == NULL) {return NULL;}
    level_build(level, levelName, sizeX, sizeY, defaultCharacter);
    return level;
}

void level_build(struct Level *level, char* levelName, int sizeX, int sizeY, char defaultCharacter) {
    level->points = calloc(MAX_ARRAY_ELEMENTS, sizeof(struct GameObject_Point*));
    if (level->points == NULL) {
        free(level);
        return;
    }

    level->rectangles = calloc(MAX_ARRAY_ELEMENTS, sizeof(struct GameObject_Rectangle*));
    if (level->rectangles == NULL) {
        free(level->points);
        free(level);
        return;
    }

    level->ellipses = calloc(MAX_ARRAY_ELEMENTS, sizeof(struct GameObject_Ellipse*));
    if (level->ellipses == NULL) {
        free(level->rectangles);
        free(level->points);
        free(level);
        return;
    }

    level->players = calloc(MAX_ARRAY_ELEMENTS, sizeof(struct GameObject_Player*));
    if (level->players == NULL) {
        free(level->ellipses);
        free(level->rectangles);
        free(level->points);
        free(level);
        return;
    }

    level->texts = calloc(MAX_ARRAY_ELEMENTS, sizeof(struct GameObject_Text*));
    if (level->texts == NULL) {
        free(level->players);
        free(level->ellipses);
        free(level->rectangles);
        free(level->points);
        free(level);
        return;
    }

    level->name = levelName;
    level->sizeX = sizeX;
    level->sizeY = sizeY;
    level->defaultCharacter = defaultCharacter;
    level->default_fg = COLOR_DEFAULT;
    level->default_bg = COLOR_DEFAULT;

    for (int i = 0; i < MAX_HUD_LINES; i++) {
        level->hud_top[i].is_active = 0;
        level->hud_top[i].text[0] = '\0';
        level->hud_top[i].fg = COLOR_DEFAULT;
        level->hud_top[i].bg = COLOR_DEFAULT;
        level->hud_bottom[i].is_active = 0;
        level->hud_bottom[i].text[0] = '\0';
        level->hud_bottom[i].fg = COLOR_DEFAULT;
        level->hud_bottom[i].bg = COLOR_DEFAULT;
    }
    level->hud_top_separator = '\0';
    level->hud_top_sep_fg = COLOR_DEFAULT;
    level->hud_top_sep_bg = COLOR_DEFAULT;
    level->hud_bottom_separator = '\0';
    level->hud_bottom_sep_fg = COLOR_DEFAULT;
    level->hud_bottom_sep_bg = COLOR_DEFAULT;
}

void level_display(struct Level *level) {
    clearConsoleScreen();
    
    int max_cell_size = 24;
    int buffer_size = (level->sizeY + MAX_HUD_LINES * 2 + 4) * (level->sizeX * max_cell_size + 64) + 1024;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) return;
    int buf_idx = 0;

    Color current_fg = COLOR_DEFAULT;
    Color current_bg = COLOR_DEFAULT;

    for (int i = 0; i < MAX_HUD_LINES; i++) {
        if (level->hud_top[i].is_active) {
            if (level->hud_top[i].fg != current_fg) {
                const char *fg_ansi = color_to_ansi_fg(level->hud_top[i].fg);
                while (*fg_ansi) buffer[buf_idx++] = *fg_ansi++;
                current_fg = level->hud_top[i].fg;
            }
            if (level->hud_top[i].bg != current_bg) {
                const char *bg_ansi = color_to_ansi_bg(level->hud_top[i].bg);
                while (*bg_ansi) buffer[buf_idx++] = *bg_ansi++;
                current_bg = level->hud_top[i].bg;
            }
            const char *str = level->hud_top[i].text;
            while (*str) buffer[buf_idx++] = *str++;
            if (current_fg != COLOR_DEFAULT || current_bg != COLOR_DEFAULT) {
                const char *reset_ansi = color_reset_ansi();
                while (*reset_ansi) buffer[buf_idx++] = *reset_ansi++;
                current_fg = COLOR_DEFAULT;
                current_bg = COLOR_DEFAULT;
            }
            buffer[buf_idx++] = '\n';
        }
    }

    if (level->hud_top_separator != '\0') {
        if (level->hud_top_sep_fg != current_fg) {
            const char *fg_ansi = color_to_ansi_fg(level->hud_top_sep_fg);
            while (*fg_ansi) buffer[buf_idx++] = *fg_ansi++;
            current_fg = level->hud_top_sep_fg;
        }
        if (level->hud_top_sep_bg != current_bg) {
            const char *bg_ansi = color_to_ansi_bg(level->hud_top_sep_bg);
            while (*bg_ansi) buffer[buf_idx++] = *bg_ansi++;
            current_bg = level->hud_top_sep_bg;
        }
        for (int x = 0; x < level->sizeX; x++) {
            buffer[buf_idx++] = level->hud_top_separator;
        }
        if (current_fg != COLOR_DEFAULT || current_bg != COLOR_DEFAULT) {
            const char *reset_ansi = color_reset_ansi();
            while (*reset_ansi) buffer[buf_idx++] = *reset_ansi++;
            current_fg = COLOR_DEFAULT;
            current_bg = COLOR_DEFAULT;
        }
        buffer[buf_idx++] = '\n';
    }

    for (int y = 0; y < level->sizeY; y++) {
        for (int x = 0; x < level->sizeX; x++) {
            char toPrint = level->defaultCharacter;
            Color cell_fg = level->default_fg;
            Color cell_bg = level->default_bg;
            
            for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
                struct GameObject_Ellipse *ell = level->ellipses[i];
                if (ell != NULL) {
                    double cx = ell->position.x + (ell->width - 1) / 2.0;
                    double cy = ell->position.y + (ell->height - 1) / 2.0;
                    double rx = ell->width / 2.0;
                    double ry = ell->height / 2.0;
                    double dx_norm = (x - cx) / (rx > 0 ? rx : 1.0);
                    double dy_norm = (y - cy) / (ry > 0 ? ry : 1.0);
                    double val = dx_norm * dx_norm + dy_norm * dy_norm;
                    
                    int inside = (val <= 1.0);
                    int border = (val >= 0.5 && val <= 1.2);

                    if ((ell->filled && inside) || (!ell->filled && border && inside)) {
                        if (ell->texture != NULL) {
                            int tx = (x - ell->position.x) % ell->texture->width;
                            int ty = (y - ell->position.y) % ell->texture->height;
                            char tc = texture_get_pixel(ell->texture, tx, ty);
                            if (tc != ell->texture->transparent_char) {
                                toPrint = tc;
                                cell_fg = ell->fg;
                                cell_bg = ell->bg;
                                break;
                            }
                        } else if (ell->character != '\0') {
                            toPrint = ell->character;
                            cell_fg = ell->fg;
                            cell_bg = ell->bg;
                            break;
                        }
                    }
                }
            }
            
            for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
                struct GameObject_Rectangle *rect = level->rectangles[i];
                if (rect != NULL) {
                    if (x >= rect->position.x && x < rect->position.x + rect->width && 
                        y >= rect->position.y && y < rect->position.y + rect->height) {
                        
                        int is_border = (x == rect->position.x || x == rect->position.x + rect->width - 1 ||
                                         y == rect->position.y || y == rect->position.y + rect->height - 1);
                        
                        if (rect->filled || is_border || rect->width <= 2 || rect->height <= 2) {
                            if (rect->texture != NULL) {
                                int tx = (x - rect->position.x) % rect->texture->width;
                                int ty = (y - rect->position.y) % rect->texture->height;
                                char tc = texture_get_pixel(rect->texture, tx, ty);
                                if (tc != rect->texture->transparent_char) {
                                    toPrint = tc;
                                    cell_fg = rect->fg;
                                    cell_bg = rect->bg;
                                }
                            } else if (rect->character != '\0') {
                                toPrint = rect->character;
                                cell_fg = rect->fg;
                                cell_bg = rect->bg;
                            }
                        }
                    }
                }
            }
            
            for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
                struct GameObject_Point *pt = level->points[i];
                if (pt != NULL && pt->character != '\0') {
                    if (pt->position.x == x && pt->position.y == y) {
                        toPrint = pt->character;
                        cell_fg = pt->fg;
                        cell_bg = pt->bg;
                    }
                }
            }

            for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
                struct GameObject_Player *pl = level->players[i];
                if (pl != NULL) {
                    if (pl->texture != NULL) {
                        if (x >= pl->position.x && x < pl->position.x + pl->texture->width &&
                            y >= pl->position.y && y < pl->position.y + pl->texture->height) {
                            int px = x - pl->position.x;
                            int py = y - pl->position.y;
                            char tc = texture_get_pixel(pl->texture, px, py);
                            if (tc != pl->texture->transparent_char) {
                                toPrint = tc;
                                cell_fg = pl->fg;
                                cell_bg = pl->bg;
                            }
                        }
                    } else if (pl->character != '\0') {
                        if (pl->position.x == x && pl->position.y == y) {
                            toPrint = pl->character;
                            cell_fg = pl->fg;
                            cell_bg = pl->bg;
                        }
                    }
                }
            }

            if (level->texts != NULL) {
                for (int t = 0; t < MAX_ARRAY_ELEMENTS; t++) {
                    struct GameObject_Text *txt = level->texts[t];
                    if (txt != NULL && txt->text[0] != '\0' && y == txt->position.y) {
                        int len = (int)strlen(txt->text);
                        if (x >= txt->position.x && x < txt->position.x + len) {
                            toPrint = txt->text[x - txt->position.x];
                            cell_fg = txt->fg;
                            cell_bg = txt->bg;
                        }
                    }
                }
            }
            
            if (cell_fg != current_fg) {
                const char *fg_ansi = color_to_ansi_fg(cell_fg);
                while (*fg_ansi) buffer[buf_idx++] = *fg_ansi++;
                current_fg = cell_fg;
            }
            if (cell_bg != current_bg) {
                const char *bg_ansi = color_to_ansi_bg(cell_bg);
                while (*bg_ansi) buffer[buf_idx++] = *bg_ansi++;
                current_bg = cell_bg;
            }
            buffer[buf_idx++] = toPrint;
        }
        if (current_fg != COLOR_DEFAULT || current_bg != COLOR_DEFAULT) {
            const char *reset_ansi = color_reset_ansi();
            while (*reset_ansi) buffer[buf_idx++] = *reset_ansi++;
            current_fg = COLOR_DEFAULT;
            current_bg = COLOR_DEFAULT;
        }
        buffer[buf_idx++] = '\n';
    }

    if (level->hud_bottom_separator != '\0') {
        if (level->hud_bottom_sep_fg != current_fg) {
            const char *fg_ansi = color_to_ansi_fg(level->hud_bottom_sep_fg);
            while (*fg_ansi) buffer[buf_idx++] = *fg_ansi++;
            current_fg = level->hud_bottom_sep_fg;
        }
        if (level->hud_bottom_sep_bg != current_bg) {
            const char *bg_ansi = color_to_ansi_bg(level->hud_bottom_sep_bg);
            while (*bg_ansi) buffer[buf_idx++] = *bg_ansi++;
            current_bg = level->hud_bottom_sep_bg;
        }
        for (int x = 0; x < level->sizeX; x++) {
            buffer[buf_idx++] = level->hud_bottom_separator;
        }
        if (current_fg != COLOR_DEFAULT || current_bg != COLOR_DEFAULT) {
            const char *reset_ansi = color_reset_ansi();
            while (*reset_ansi) buffer[buf_idx++] = *reset_ansi++;
            current_fg = COLOR_DEFAULT;
            current_bg = COLOR_DEFAULT;
        }
        buffer[buf_idx++] = '\n';
    }

    for (int i = 0; i < MAX_HUD_LINES; i++) {
        if (level->hud_bottom[i].is_active) {
            if (level->hud_bottom[i].fg != current_fg) {
                const char *fg_ansi = color_to_ansi_fg(level->hud_bottom[i].fg);
                while (*fg_ansi) buffer[buf_idx++] = *fg_ansi++;
                current_fg = level->hud_bottom[i].fg;
            }
            if (level->hud_bottom[i].bg != current_bg) {
                const char *bg_ansi = color_to_ansi_bg(level->hud_bottom[i].bg);
                while (*bg_ansi) buffer[buf_idx++] = *bg_ansi++;
                current_bg = level->hud_bottom[i].bg;
            }
            const char *str = level->hud_bottom[i].text;
            while (*str) buffer[buf_idx++] = *str++;
            if (current_fg != COLOR_DEFAULT || current_bg != COLOR_DEFAULT) {
                const char *reset_ansi = color_reset_ansi();
                while (*reset_ansi) buffer[buf_idx++] = *reset_ansi++;
                current_fg = COLOR_DEFAULT;
                current_bg = COLOR_DEFAULT;
            }
            buffer[buf_idx++] = '\n';
        }
    }

    if (current_fg != COLOR_DEFAULT || current_bg != COLOR_DEFAULT) {
        const char *reset_ansi = color_reset_ansi();
        while (*reset_ansi) buffer[buf_idx++] = *reset_ansi++;
    }
    buffer[buf_idx] = '\0';
    
    printf("%s", buffer);
    fflush(stdout);
    
    free(buffer);
}

void level_add_point(struct Level *level, struct GameObject_Point *point) {
    if (level == NULL || point == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->points[i] == NULL) {
            level->points[i] = point;
            return;
        }
    }
}

void level_remove_point(struct Level *level, struct GameObject_Point *point) {
    if (level == NULL || point == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->points[i] == point) {
            level->points[i] = NULL;
            return;
        }
    }
}

struct GameObject_Point* level_get_point(struct Level *level, struct Vector2 position) {
    if (level == NULL) return NULL;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Point *pt = level->points[i];
        if (pt != NULL && pt->character != '\0' && pt->position.x == position.x && pt->position.y == position.y) {
            return pt;
        }
    }
    return NULL;
}

void level_add_rectangle(struct Level *level, struct GameObject_Rectangle *rectangle) {
    if (level == NULL || rectangle == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->rectangles[i] == NULL) {
            level->rectangles[i] = rectangle;
            break;
        }
    }
}

void level_remove_rectangle(struct Level *level, struct GameObject_Rectangle *rectangle) {
    if (level == NULL || rectangle == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->rectangles[i] == rectangle) {
            level->rectangles[i] = NULL;
            break;
        }
    }
}

struct GameObject_Rectangle* level_get_rectangle(struct Level *level, struct Vector2 position) {
    if (level == NULL) return NULL;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Rectangle *rect = level->rectangles[i];
        if (rect != NULL && rect->character != '\0') {
            if (position.x >= rect->position.x && position.x < rect->position.x + rect->width &&
                position.y >= rect->position.y && position.y < rect->position.y + rect->height) {
                return rect;
            }
        }
    }
    return NULL;
}

void level_add_ellipse(struct Level *level, struct GameObject_Ellipse *ellipse) {
    if (level == NULL || ellipse == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->ellipses[i] == NULL) {
            level->ellipses[i] = ellipse;
            break;
        }
    }
}

void level_remove_ellipse(struct Level *level, struct GameObject_Ellipse *ellipse) {
    if (level == NULL || ellipse == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->ellipses[i] == ellipse) {
            level->ellipses[i] = NULL;
            break;
        }
    }
}

struct GameObject_Ellipse* level_get_ellipse(struct Level *level, struct Vector2 position) {
    if (level == NULL) return NULL;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Ellipse *ell = level->ellipses[i];
        if (ell != NULL && ell->character != '\0') {
            if (position.x >= ell->position.x && position.x < ell->position.x + ell->width &&
                position.y >= ell->position.y && position.y < ell->position.y + ell->height) {
                
                double cx = ell->position.x + (ell->width - 1) / 2.0;
                double cy = ell->position.y + (ell->height - 1) / 2.0;
                double rx = ell->width / 2.0;
                double ry = ell->height / 2.0;
                double dx_norm = (position.x - cx) / (rx > 0 ? rx : 1.0);
                double dy_norm = (position.y - cy) / (ry > 0 ? ry : 1.0);
                double val = dx_norm * dx_norm + dy_norm * dy_norm;
                
                if (val <= 1.0) {
                    return ell;
                }
            }
        }
    }
    return NULL;
}

void level_add_player(struct Level *level, struct GameObject_Player *player) {
    if (level == NULL || player == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->players[i] == NULL) {
            level->players[i] = player;
            break;
        }
    }
}

void level_remove_player(struct Level *level, struct GameObject_Player *player) {
    if (level == NULL || player == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->players[i] == player) {
            level->players[i] = NULL;
            break;
        }
    }
}

struct GameObject_Player* level_get_player(struct Level *level, struct Vector2 position) {
    if (level == NULL) return NULL;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        struct GameObject_Player *pl = level->players[i];
        if (pl != NULL) {
            if (pl->texture != NULL) {
                if (position.x >= pl->position.x && position.x < pl->position.x + pl->texture->width &&
                    position.y >= pl->position.y && position.y < pl->position.y + pl->texture->height) {
                    return pl;
                }
            } else if (pl->character != '\0') {
                if (pl->position.x == position.x && pl->position.y == position.y) {
                    return pl;
                }
            }
        }
    }
    return NULL;
}

void level_add_text(struct Level *level, struct GameObject_Text *text) {
    if (level == NULL || text == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->texts[i] == NULL) {
            level->texts[i] = text;
            return;
        }
    }
}

void level_remove_text(struct Level *level, struct GameObject_Text *text) {
    if (level == NULL || text == NULL) return;
    for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
        if (level->texts[i] == text) {
            level->texts[i] = NULL;
            return;
        }
    }
}

void level_set_hud_text(struct Level *level, HudPosition pos, int line_index, const char *text, Color fg, Color bg) {
    if (level == NULL || line_index < 0 || line_index >= MAX_HUD_LINES) return;
    struct HudLine *line = (pos == HUD_TOP) ? &level->hud_top[line_index] : &level->hud_bottom[line_index];
    if (text == NULL || text[0] == '\0') {
        line->is_active = 0;
        line->text[0] = '\0';
    } else {
        line->is_active = 1;
        strncpy(line->text, text, sizeof(line->text) - 1);
        line->text[sizeof(line->text) - 1] = '\0';
        line->fg = fg;
        line->bg = bg;
    }
}

void level_clear_hud(struct Level *level, HudPosition pos) {
    if (level == NULL) return;
    struct HudLine *target = (pos == HUD_TOP) ? level->hud_top : level->hud_bottom;
    for (int i = 0; i < MAX_HUD_LINES; i++) {
        target[i].is_active = 0;
        target[i].text[0] = '\0';
    }
}

void level_set_hud_separator(struct Level *level, HudPosition pos, char separator_char, Color fg, Color bg) {
    if (level == NULL) return;
    if (pos == HUD_TOP) {
        level->hud_top_separator = separator_char;
        level->hud_top_sep_fg = fg;
        level->hud_top_sep_bg = bg;
    } else {
        level->hud_bottom_separator = separator_char;
        level->hud_bottom_sep_fg = fg;
        level->hud_bottom_sep_bg = bg;
    }
}

char* level_get_name(struct Level *level) {
    return level->name;
}

int level_get_sizeX(struct Level *level) {
    return level->sizeX;
}

int level_get_sizeY(struct Level *level) {
    return level->sizeY;
}

char level_get_defaultCharacter(struct Level *level) {
    return level->defaultCharacter;
}

void level_set_name(struct Level *level, char* name) {
    level->name = name;
}

void level_set_sizeX(struct Level *level, int sizeX) {
    level->sizeX = sizeX;
}

void level_set_sizeY(struct Level *level, int sizeY) {
    level->sizeY = sizeY;
}

void level_set_defaultCharacter(struct Level *level, char defaultCharacter) {
    level->defaultCharacter = defaultCharacter;
}

void level_set_default_color(struct Level *level, Color fg, Color bg) {
    if (level == NULL) return;
    level->default_fg = fg;
    level->default_bg = bg;
}

Color level_get_default_fg(struct Level *level) {
    if (level == NULL) return COLOR_DEFAULT;
    return level->default_fg;
}

Color level_get_default_bg(struct Level *level) {
    if (level == NULL) return COLOR_DEFAULT;
    return level->default_bg;
}

void level_free(struct Level *level) {
    if (level == NULL) return;
    
    if (level->points != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (level->points[i] != NULL) {
                point_free(level->points[i]); 
            }
        }
        free(level->points); 
    }
    
    if (level->rectangles != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (level->rectangles[i] != NULL) {
                rectangle_free(level->rectangles[i]);
            }
        }
        free(level->rectangles);
    }
    
    if (level->ellipses != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (level->ellipses[i] != NULL) {
                ellipse_free(level->ellipses[i]);
            }
        }
        free(level->ellipses);
    }

    if (level->players != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (level->players[i] != NULL) {
                player_free(level->players[i]);
            }
        }
        free(level->players);
    }

    if (level->texts != NULL) {
        for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
            if (level->texts[i] != NULL) {
                text_free(level->texts[i]);
            }
        }
        free(level->texts);
    }
    
    free(level);
}