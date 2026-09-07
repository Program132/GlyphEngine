#include "level.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/utils.h"

struct ScreenCell {
    char ch;
    Color fg;
    Color bg;
};

static struct ScreenCell *g_prev_cells = NULL;
static int g_prev_cols = 0;
static int g_prev_rows = 0;

void level_reset_screen_buffer(void) {
    if (g_prev_cells != NULL) {
        free(g_prev_cells);
        g_prev_cells = NULL;
    }
    g_prev_cols = 0;
    g_prev_rows = 0;
}

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

    for (int i = 0; i < MAX_LEVEL_PROJECTILES; i++) {
        level->projectiles[i] = NULL;
    }
    level->particle_system = particle_system_new();
    level->is_camera_enabled = 0;
    level->viewport_w = sizeX;
    level->viewport_h = sizeY;
    level->cam_x = 0.0f;
    level->cam_y = 0.0f;
}

void level_display(struct Level *level) {
    int render_w = level->is_camera_enabled ? level->viewport_w : level->sizeX;
    int render_h = level->is_camera_enabled ? level->viewport_h : level->sizeY;
    int cam_offset_x = level->is_camera_enabled ? (int)level->cam_x : 0;
    int cam_offset_y = level->is_camera_enabled ? (int)level->cam_y : 0;

    int total_rows = 0;
    for (int i = 0; i < MAX_HUD_LINES; i++) {
        if (level->hud_top[i].is_active) total_rows++;
    }
    if (level->hud_top_separator != '\0') total_rows++;
    total_rows += render_h;
    if (level->hud_bottom_separator != '\0') total_rows++;
    for (int i = 0; i < MAX_HUD_LINES; i++) {
        if (level->hud_bottom[i].is_active) total_rows++;
    }
    int total_cols = render_w;

    struct ScreenCell *curr_cells = malloc(sizeof(struct ScreenCell) * total_rows * total_cols);
    if (curr_cells == NULL) return;

    int cur_r = 0;

    for (int i = 0; i < MAX_HUD_LINES; i++) {
        if (level->hud_top[i].is_active) {
            const char *str = level->hud_top[i].text;
            int slen = (int)strlen(str);
            for (int c = 0; c < total_cols; c++) {
                char ch = (c < slen) ? str[c] : ' ';
                curr_cells[cur_r * total_cols + c] = (struct ScreenCell){ch, level->hud_top[i].fg, level->hud_top[i].bg};
            }
            cur_r++;
        }
    }

    if (level->hud_top_separator != '\0') {
        for (int c = 0; c < total_cols; c++) {
            curr_cells[cur_r * total_cols + c] = (struct ScreenCell){level->hud_top_separator, level->hud_top_sep_fg, level->hud_top_sep_bg};
        }
        cur_r++;
    }

    for (int vy = 0; vy < render_h; vy++) {
        int y = vy + cam_offset_y;
        for (int vx = 0; vx < render_w; vx++) {
            int x = vx + cam_offset_x;
            char toPrint = (x >= 0 && x < level->sizeX && y >= 0 && y < level->sizeY) ? level->defaultCharacter : ' ';
            Color cell_fg = (x >= 0 && x < level->sizeX && y >= 0 && y < level->sizeY) ? level->default_fg : COLOR_DEFAULT;
            Color cell_bg = (x >= 0 && x < level->sizeX && y >= 0 && y < level->sizeY) ? level->default_bg : COLOR_DEFAULT;

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

            for (int p = 0; p < MAX_LEVEL_PROJECTILES; p++) {
                struct GameObject_Projectile *proj = level->projectiles[p];
                if (proj != NULL && proj->is_alive && proj->position.x == x && proj->position.y == y) {
                    toPrint = proj->character;
                    cell_fg = proj->fg;
                    cell_bg = proj->bg;
                }
            }

            if (level->particle_system != NULL) {
                for (int pt = 0; pt < MAX_PARTICLES; pt++) {
                    struct Particle *part = &level->particle_system->particles[pt];
                    if (part->is_alive && part->x == x && part->y == y) {
                        toPrint = part->character;
                        cell_fg = part->fg;
                        cell_bg = part->bg;
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

            curr_cells[cur_r * total_cols + vx] = (struct ScreenCell){toPrint, cell_fg, cell_bg};
        }
        cur_r++;
    }

    if (level->hud_bottom_separator != '\0') {
        for (int c = 0; c < total_cols; c++) {
            curr_cells[cur_r * total_cols + c] = (struct ScreenCell){level->hud_bottom_separator, level->hud_bottom_sep_fg, level->hud_bottom_sep_bg};
        }
        cur_r++;
    }

    for (int i = 0; i < MAX_HUD_LINES; i++) {
        if (level->hud_bottom[i].is_active) {
            const char *str = level->hud_bottom[i].text;
            int slen = (int)strlen(str);
            for (int c = 0; c < total_cols; c++) {
                char ch = (c < slen) ? str[c] : ' ';
                curr_cells[cur_r * total_cols + c] = (struct ScreenCell){ch, level->hud_bottom[i].fg, level->hud_bottom[i].bg};
            }
            cur_r++;
        }
    }

    int buf_cap = total_rows * (total_cols * 32 + 32) + 4096;
    char *out_buf = malloc(buf_cap);
    if (out_buf == NULL) {
        free(curr_cells);
        return;
    }
    int out_idx = 0;

    int need_full_redraw = (g_prev_cells == NULL || g_prev_rows != total_rows || g_prev_cols != total_cols);

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
                if (curr_cells[idx].ch != g_prev_cells[idx].ch ||
                    curr_cells[idx].fg != g_prev_cells[idx].fg ||
                    curr_cells[idx].bg != g_prev_cells[idx].bg) {

                    out_idx += snprintf(&out_buf[out_idx], buf_cap - out_idx, "\033[%d;%dH", r + 1, c + 1);

                    while (c < total_cols) {
                        int cidx = r * total_cols + c;
                        if (curr_cells[cidx].ch == g_prev_cells[cidx].ch &&
                            curr_cells[cidx].fg == g_prev_cells[cidx].fg &&
                            curr_cells[cidx].bg == g_prev_cells[cidx].bg) {
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

    if (g_prev_cells == NULL || g_prev_rows != total_rows || g_prev_cols != total_cols) {
        if (g_prev_cells != NULL) free(g_prev_cells);
        g_prev_cells = malloc(sizeof(struct ScreenCell) * total_rows * total_cols);
        g_prev_rows = total_rows;
        g_prev_cols = total_cols;
    }

    if (g_prev_cells != NULL) {
        memcpy(g_prev_cells, curr_cells, sizeof(struct ScreenCell) * total_rows * total_cols);
    }

    free(curr_cells);
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

void level_spawn_projectile(struct Level *level, float x, float y, float vx, float vy, char character, Color fg, int damage, float lifetime, ProjectileOwner owner) {
    if (level == NULL) return;
    int slot = -1;
    for (int i = 0; i < MAX_LEVEL_PROJECTILES; i++) {
        if (level->projectiles[i] == NULL) {
            slot = i;
            break;
        } else if (!level->projectiles[i]->is_alive) {
            slot = i;
            break;
        }
    }
    if (slot == -1) return;

    if (level->projectiles[slot] == NULL) {
        level->projectiles[slot] = projectile_new(x, y, vx, vy, character, fg, COLOR_DEFAULT, damage, lifetime, owner);
    } else {
        projectile_build(level->projectiles[slot], x, y, vx, vy, character, fg, COLOR_DEFAULT, damage, lifetime, owner);
    }
}

void level_spawn_particles_explosion(struct Level *level, float x, float y, int count, Color fg) {
    if (level == NULL || level->particle_system == NULL) return;
    particle_system_burst_explosion(level->particle_system, x, y, count, fg);
}

void level_spawn_particles_sparkle(struct Level *level, float x, float y, int count, Color fg) {
    if (level == NULL || level->particle_system == NULL) return;
    particle_system_burst_sparkle(level->particle_system, x, y, count, fg);
}

void level_update(struct Level *level, float dt) {
    if (level == NULL) return;
    for (int i = 0; i < MAX_LEVEL_PROJECTILES; i++) {
        struct GameObject_Projectile *p = level->projectiles[i];
        if (p != NULL && p->is_alive) {
            projectile_update(p, dt);
            if (p->position.x < 0 || p->position.x >= level->sizeX ||
                p->position.y < 0 || p->position.y >= level->sizeY) {
                p->is_alive = 0;
            }
        }
    }
    if (level->particle_system != NULL) {
        particle_system_update(level->particle_system, dt);
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

void level_set_camera(struct Level *level, float cam_x, float cam_y) {
    if (level == NULL) return;
    level->cam_x = cam_x;
    level->cam_y = cam_y;
}

void level_enable_camera(struct Level *level, int viewport_w, int viewport_h) {
    if (level == NULL) return;
    level->is_camera_enabled = 1;
    level->viewport_w = viewport_w;
    level->viewport_h = viewport_h;
}

void level_disable_camera(struct Level *level) {
    if (level == NULL) return;
    level->is_camera_enabled = 0;
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

    for (int i = 0; i < MAX_LEVEL_PROJECTILES; i++) {
        if (level->projectiles[i] != NULL) {
            projectile_free(level->projectiles[i]);
            level->projectiles[i] = NULL;
        }
    }

    if (level->particle_system != NULL) {
        particle_system_free(level->particle_system);
    }
    
    free(level);
}