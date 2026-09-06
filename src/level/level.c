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

    level->name = levelName;
    level->sizeX = sizeX;
    level->sizeY = sizeY;
    level->defaultCharacter = defaultCharacter;
}

void level_display(struct Level *level) {
    clearConsoleScreen();
    
    int row_len = level->sizeX + 1;
    int buffer_size = row_len * level->sizeY + 1;
    char *buffer = malloc(buffer_size);
    if (buffer == NULL) return;

    for (int y = 0; y < level->sizeY; y++) {
        for (int x = 0; x < level->sizeX; x++) {
            char toPrint = level->defaultCharacter;
            
            for (int i = 0; i < MAX_ARRAY_ELEMENTS; i++) {
                struct GameObject_Ellipse *ell = level->ellipses[i];
                if (ell != NULL && ell->character != '\0') {
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
                        toPrint = ell->character;
                        break;
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
                                }
                            } else if (rect->character != '\0') {
                                toPrint = rect->character;
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
                            }
                        }
                    } else if (pl->character != '\0') {
                        if (pl->position.x == x && pl->position.y == y) {
                            toPrint = pl->character;
                        }
                    }
                }
            }
            
            buffer[y * row_len + x] = toPrint;
        }
        buffer[y * row_len + level->sizeX] = '\n';
    }
    buffer[buffer_size - 1] = '\0';
    
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
    
    free(level);
}