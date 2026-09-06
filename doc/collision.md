# Collision Module

The `Collision` module provides 2D collision detection routines between geometric shapes, players, and scene boundaries.

# Functions

### Geometric Intersections

| Name                              | Type | Arguments                                                                      | Description                                                                              |
|-----------------------------------|------|--------------------------------------------------------------------------------|------------------------------------------------------------------------------------------|
| collision_check_point_rect        | int  | struct Vector2 point, struct GameObject_Rectangle *rect                        | Returns `1` if the 2D point is within the rectangle's bounds, `0` otherwise.            |
| collision_check_rect_rect         | int  | struct GameObject_Rectangle *r1, struct GameObject_Rectangle *r2               | Returns `1` if two rectangles intersect (AABB collision), `0` otherwise.                |
| collision_check_point_ellipse     | int  | struct Vector2 point, struct GameObject_Ellipse *ellipse                       | Returns `1` if the point is inside the ellipse or circle, `0` otherwise.                 |
| collision_check_rect_ellipse      | int  | struct GameObject_Rectangle *rect, struct GameObject_Ellipse *ellipse          | Returns `1` if the rectangle and ellipse intersect, `0` otherwise.                       |
| collision_check_ellipse_ellipse   | int  | struct GameObject_Ellipse *e1, struct GameObject_Ellipse *e2                   | Returns `1` if two ellipses/circles intersect, `0` otherwise.                            |

### Player Intersections

| Name                             | Type | Arguments                                                                      | Description                                                                              |
|----------------------------------|------|--------------------------------------------------------------------------------|------------------------------------------------------------------------------------------|
| collision_check_player_rect       | int  | struct GameObject_Player *player, struct GameObject_Rectangle *rect            | Returns `1` if the player intersects the rectangle, `0` otherwise.                      |
| collision_check_player_ellipse    | int  | struct GameObject_Player *player, struct GameObject_Ellipse *ellipse           | Returns `1` if the player intersects the ellipse or circle, `0` otherwise.              |
| collision_check_player_point      | int  | struct GameObject_Player *player, struct GameObject_Point *point               | Returns `1` if the player touches the point (respecting texture transparency), `0` else. |
| collision_check_player_player     | int  | struct GameObject_Player *p1, struct GameObject_Player *p2                     | Returns `1` if two players intersect, `0` otherwise.                                     |

### Boundary Control

| Name                             | Type | Arguments                                                                      | Description                                                                              |
|----------------------------------|------|--------------------------------------------------------------------------------|------------------------------------------------------------------------------------------|
| collision_is_out_of_bounds       | int  | struct Level *level, int x, int y                                              | Returns `1` if coordinate (x, y) is outside level boundaries, `0` otherwise.             |
| collision_clamp_player           | void | struct Level *level, struct GameObject_Player *player                          | Restricts player position within level boundaries, accounting for texture dimensions.     |

# Usage Example

```c
#include "collision/collision.h"

// Stop player movement upon hitting a wall
if (collision_check_player_rect(player, wall)) {
    player_set_position(player, previous_x, previous_y);
}

// Keep player on screen
collision_clamp_player(level, player);
```
