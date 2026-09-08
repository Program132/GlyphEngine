# Light Module

The `Light` module brings real-time 2D dynamic lighting, shadows (line-of-sight raycasting), and flame flicker effects to GlyphEngine.

## Overview

- **Point Lights**: Omnidirectional light sources with custom position, radius, base intensity, and 24-bit TrueColor hue.
- **Dynamic Shadows**: Solid obstacles (`is_solid = 1`) cast realistic hard shadows by blocking light rays using Bresenham line-of-sight raycasting.
- **Torch Flicker**: Automatic organic sinusoidal flicker simulation with configurable amount and speed.
- **Ambient Darkness**: Global ambient color can be adjusted from subtle dimming (`color_rgb(30, 30, 45)`) to complete pitch-black darkness (`color_rgb(0, 0, 0)`).
- **100% Backward Compatible**: Disabled by default (`lighting_enabled = 0`). Only active when calling `level_enable_lighting(level, 1)`.
- **HUD Preservation**: Top and bottom HUD lines and separators remain crisp and unaffected by world lighting.

---

## Types

### `struct Light`

| Field            | Type    | Description                                            |
|------------------|---------|--------------------------------------------------------|
| `x`, `y`         | float   | World coordinate position of the light center.         |
| `radius`         | float   | Maximum radius of the light reach in tiles.            |
| `intensity`      | float   | Base intensity multiplier (e.g. 1.0f).                 |
| `color`          | Color   | TrueColor RGB or ANSI color of the emitted light.      |
| `flicker_amount` | float   | Intensity variation magnitude (e.g. 0.15f for torch).  |
| `flicker_speed`  | float   | Frequency of the flicker oscillation (e.g. 5.0f).      |
| `flicker_timer`  | float   | Internal oscillation accumulator.                      |
| `is_active`      | int     | 1 if light is emitting, 0 if toggled off.              |
| `cast_shadows`   | int     | 1 if blocked by solid walls, 0 for ambient pass-through.|

---

## Functions

### Light Management (`light.h`)

| Name                        | Type           | Arguments                                                      | Description                                             |
|-----------------------------|----------------|----------------------------------------------------------------|---------------------------------------------------------|
| `light_new`                 | `struct Light*`| `float x, float y, float radius, float intensity, Color color` | Allocates and initializes a new Light object.           |
| `light_build`               | `void`         | `struct Light *l, float x, float y, float r, float i, Color c` | Initializes a pre-allocated Light struct.              |
| `light_free`                | `void`         | `struct Light *l`                                              | Frees the Light object.                                 |
| `light_set_position`        | `void`         | `struct Light *l, float x, float y`                            | Updates the light position (e.g. tracking player).      |
| `light_set_radius`          | `void`         | `struct Light *l, float radius`                                | Sets the maximum reach radius.                          |
| `light_set_intensity`       | `void`         | `struct Light *l, float intensity`                             | Sets the base brightness multiplier.                    |
| `light_set_color`           | `void`         | `struct Light *l, Color color`                                 | Sets the light tint color.                              |
| `light_set_flicker`         | `void`         | `struct Light *l, float amount, float speed`                   | Configures torch flicker dynamics.                      |
| `light_set_cast_shadows`    | `void`         | `struct Light *l, int cast_shadows`                            | Enables or disables shadow casting for this light.      |
| `light_set_active`          | `void`         | `struct Light *l, int active`                                  | Toggles light emission on/off.                          |
| `light_update`              | `void`         | `struct Light *l, float dt`                                    | Advances the flicker oscillation timer.                 |
| `light_get_current_intensity`| `float`       | `struct Light *l`                                              | Computes current intensity including flicker variance.  |

### Level Lighting Integration (`level.h`)

| Name                       | Type   | Arguments                         | Description                                            |
|----------------------------|--------|-----------------------------------|--------------------------------------------------------|
| `level_enable_lighting`    | `void` | `struct Level *lvl, int enabled`  | Enables (1) or disables (0) dynamic lighting on level. |
| `level_set_ambient_light`  | `void` | `struct Level *lvl, Color ambient`| Sets the baseline global ambient color in darkness.    |
| `level_add_light`          | `void` | `struct Level *lvl, Light *light` | Adds a light source to the level (up to 32 lights).    |
| `level_remove_light`       | `void` | `struct Level *lvl, Light *light` | Removes a light from the level.                        |
| `level_clear_lights`       | `void` | `struct Level *lvl`               | Removes all light sources from the level.              |
| `level_is_solid_at`        | `int`  | `struct Level *lvl, int x, int y` | Tests if world coordinate (x,y) has a solid obstacle.  |

---

## Usage Example

```c
#include "level/level.h"
#include "light/light.h"
#include "color/color.h"

// 1. Enable lighting on the level
level_enable_lighting(level, 1);

// 2. Set dark blue ambient dungeon light
level_set_ambient_light(level, color_rgb(15, 15, 25));

// 3. Create a warm flickering wall torch
struct Light *torch = light_new(35.0f, 15.0f, 12.0f, 1.1f, color_rgb(255, 170, 40));
light_set_flicker(torch, 0.15f, 6.0f);
level_add_light(level, torch);

// 4. Create a handheld torch that follows the hero in the game loop
struct Light *hero_light = light_new(hero->exact_x, hero->exact_y, 10.0f, 1.0f, color_rgb(255, 220, 150));
level_add_light(level, hero_light);

// In update loop:
light_set_position(hero_light, hero->exact_x, hero->exact_y);
```
