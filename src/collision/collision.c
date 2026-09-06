#include "collision.h"
#include "../level/level.h"
#include <stdlib.h>

int collision_check_point_rect(struct Vector2 point, struct GameObject_Rectangle *rect) {
    if (rect == NULL) return 0;
    return (point.x >= rect->position.x &&
            point.x < rect->position.x + rect->width &&
            point.y >= rect->position.y &&
            point.y < rect->position.y + rect->height);
}

int collision_check_rect_rect(struct GameObject_Rectangle *r1, struct GameObject_Rectangle *r2) {
    if (r1 == NULL || r2 == NULL) return 0;
    return (r1->position.x < r2->position.x + r2->width &&
            r1->position.x + r1->width > r2->position.x &&
            r1->position.y < r2->position.y + r2->height &&
            r1->position.y + r1->height > r2->position.y);
}

int collision_check_point_ellipse(struct Vector2 point, struct GameObject_Ellipse *ellipse) {
    if (ellipse == NULL || ellipse->width <= 0 || ellipse->height <= 0) return 0;
    double cx = ellipse->position.x + (ellipse->width - 1) / 2.0;
    double cy = ellipse->position.y + (ellipse->height - 1) / 2.0;
    double rx = ellipse->width / 2.0;
    double ry = ellipse->height / 2.0;
    double dx = (point.x - cx) / (rx > 0 ? rx : 1.0);
    double dy = (point.y - cy) / (ry > 0 ? ry : 1.0);
    return (dx * dx + dy * dy <= 1.0);
}

int collision_check_rect_ellipse(struct GameObject_Rectangle *rect, struct GameObject_Ellipse *ellipse) {
    if (rect == NULL || ellipse == NULL) return 0;
    double cx = ellipse->position.x + (ellipse->width - 1) / 2.0;
    double cy = ellipse->position.y + (ellipse->height - 1) / 2.0;
    
    double closest_x = cx;
    if (closest_x < rect->position.x) closest_x = rect->position.x;
    else if (closest_x > rect->position.x + rect->width - 1) closest_x = rect->position.x + rect->width - 1;

    double closest_y = cy;
    if (closest_y < rect->position.y) closest_y = rect->position.y;
    else if (closest_y > rect->position.y + rect->height - 1) closest_y = rect->position.y + rect->height - 1;

    struct Vector2 pt = {(int)(closest_x + 0.5), (int)(closest_y + 0.5)};
    return collision_check_point_ellipse(pt, ellipse);
}

int collision_check_ellipse_ellipse(struct GameObject_Ellipse *e1, struct GameObject_Ellipse *e2) {
    if (e1 == NULL || e2 == NULL) return 0;
    double c1x = e1->position.x + (e1->width - 1) / 2.0;
    double c1y = e1->position.y + (e1->height - 1) / 2.0;
    double c2x = e2->position.x + (e2->width - 1) / 2.0;
    double c2y = e2->position.y + (e2->height - 1) / 2.0;
    double r1 = (e1->width + e1->height) / 4.0;
    double r2 = (e2->width + e2->height) / 4.0;
    double dx = c1x - c2x;
    double dy = c1y - c2y;
    return (dx * dx + dy * dy <= (r1 + r2) * (r1 + r2));
}

int collision_check_player_rect(struct GameObject_Player *player, struct GameObject_Rectangle *rect) {
    if (player == NULL || rect == NULL) return 0;
    int pw = (player->texture != NULL) ? player->texture->width : 1;
    int ph = (player->texture != NULL) ? player->texture->height : 1;
    struct GameObject_Rectangle pr;
    pr.position = player->position;
    pr.width = pw;
    pr.height = ph;
    return collision_check_rect_rect(&pr, rect);
}

int collision_check_player_ellipse(struct GameObject_Player *player, struct GameObject_Ellipse *ellipse) {
    if (player == NULL || ellipse == NULL) return 0;
    int pw = (player->texture != NULL) ? player->texture->width : 1;
    int ph = (player->texture != NULL) ? player->texture->height : 1;
    struct GameObject_Rectangle pr;
    pr.position = player->position;
    pr.width = pw;
    pr.height = ph;
    return collision_check_rect_ellipse(&pr, ellipse);
}

int collision_check_player_point(struct GameObject_Player *player, struct GameObject_Point *point) {
    if (player == NULL || point == NULL) return 0;
    int pw = (player->texture != NULL) ? player->texture->width : 1;
    int ph = (player->texture != NULL) ? player->texture->height : 1;
    if (point->position.x >= player->position.x &&
        point->position.x < player->position.x + pw &&
        point->position.y >= player->position.y &&
        point->position.y < player->position.y + ph) {
        if (player->texture != NULL) {
            int px = point->position.x - player->position.x;
            int py = point->position.y - player->position.y;
            return texture_get_pixel(player->texture, px, py) != player->texture->transparent_char;
        }
        return 1;
    }
    return 0;
}

int collision_check_player_player(struct GameObject_Player *p1, struct GameObject_Player *p2) {
    if (p1 == NULL || p2 == NULL) return 0;
    int p1w = (p1->texture != NULL) ? p1->texture->width : 1;
    int p1h = (p1->texture != NULL) ? p1->texture->height : 1;
    int p2w = (p2->texture != NULL) ? p2->texture->width : 1;
    int p2h = (p2->texture != NULL) ? p2->texture->height : 1;
    struct GameObject_Rectangle r1;
    r1.position = p1->position;
    r1.width = p1w;
    r1.height = p1h;
    struct GameObject_Rectangle r2;
    r2.position = p2->position;
    r2.width = p2w;
    r2.height = p2h;
    return collision_check_rect_rect(&r1, &r2);
}

int collision_check_projectile_rect(struct GameObject_Projectile *proj, struct GameObject_Rectangle *rect) {
    if (proj == NULL || !proj->is_alive || rect == NULL) return 0;
    return collision_check_point_rect(proj->position, rect);
}

int collision_check_projectile_ellipse(struct GameObject_Projectile *proj, struct GameObject_Ellipse *ellipse) {
    if (proj == NULL || !proj->is_alive || ellipse == NULL) return 0;
    return collision_check_point_ellipse(proj->position, ellipse);
}

int collision_check_projectile_player(struct GameObject_Projectile *proj, struct GameObject_Player *player) {
    if (proj == NULL || !proj->is_alive || player == NULL) return 0;
    int pw = (player->texture != NULL) ? player->texture->width : 1;
    int ph = (player->texture != NULL) ? player->texture->height : 1;
    if (proj->position.x >= player->position.x &&
        proj->position.x < player->position.x + pw &&
        proj->position.y >= player->position.y &&
        proj->position.y < player->position.y + ph) {
        if (player->texture != NULL) {
            int px = proj->position.x - player->position.x;
            int py = proj->position.y - player->position.y;
            return texture_get_pixel(player->texture, px, py) != player->texture->transparent_char;
        }
        return 1;
    }
    return 0;
}

int collision_is_out_of_bounds(struct Level *level, int x, int y) {
    if (level == NULL) return 1;
    return (x < 0 || x >= level->sizeX || y < 0 || y >= level->sizeY);
}

void collision_clamp_player(struct Level *level, struct GameObject_Player *player) {
    if (level == NULL || player == NULL) return;
    int pw = (player->texture != NULL) ? player->texture->width : 1;
    int ph = (player->texture != NULL) ? player->texture->height : 1;
    if (player->position.x < 0) {
        player_set_position(player, 0, player->position.y);
    }
    if (player->position.y < 0) {
        player_set_position(player, player->position.x, 0);
    }
    if (player->position.x + pw > level->sizeX) {
        player_set_position(player, level->sizeX - pw, player->position.y);
    }
    if (player->position.y + ph > level->sizeY) {
        player_set_position(player, player->position.x, level->sizeY - ph);
    }
}
