#include "light.h"
#include <stdlib.h>
#include <math.h>

struct Light* light_new(float x, float y, float radius, float intensity, Color color) {
    struct Light *light = (struct Light*)malloc(sizeof(struct Light));
    if (light == NULL) return NULL;
    light_build(light, x, y, radius, intensity, color);
    return light;
}

void light_build(struct Light *light, float x, float y, float radius, float intensity, Color color) {
    if (light == NULL) return;
    light->x = x;
    light->y = y;
    light->radius = radius;
    light->intensity = intensity;
    light->color = color;
    light->flicker_amount = 0.0f;
    light->flicker_speed = 5.0f;
    light->flicker_timer = 0.0f;
    light->is_active = 1;
    light->cast_shadows = 1;
}

void light_free(struct Light *light) {
    if (light != NULL) {
        free(light);
    }
}

void light_set_position(struct Light *light, float x, float y) {
    if (light == NULL) return;
    light->x = x;
    light->y = y;
}

void light_set_radius(struct Light *light, float radius) {
    if (light == NULL) return;
    light->radius = radius > 0.0f ? radius : 0.0f;
}

void light_set_intensity(struct Light *light, float intensity) {
    if (light == NULL) return;
    light->intensity = intensity > 0.0f ? intensity : 0.0f;
}

void light_set_color(struct Light *light, Color color) {
    if (light == NULL) return;
    light->color = color;
}

void light_set_flicker(struct Light *light, float amount, float speed) {
    if (light == NULL) return;
    light->flicker_amount = amount > 0.0f ? amount : 0.0f;
    light->flicker_speed = speed > 0.0f ? speed : 1.0f;
}

void light_set_cast_shadows(struct Light *light, int cast_shadows) {
    if (light == NULL) return;
    light->cast_shadows = cast_shadows;
}

void light_set_active(struct Light *light, int active) {
    if (light == NULL) return;
    light->is_active = active;
}

void light_update(struct Light *light, float dt) {
    if (light == NULL || !light->is_active || dt <= 0.0f) return;
    if (light->flicker_amount > 0.0f) {
        light->flicker_timer += dt * light->flicker_speed;
        if (light->flicker_timer > 62.8318f) {
            light->flicker_timer -= 62.8318f;
        }
    }
}

float light_get_current_intensity(struct Light *light) {
    if (light == NULL || !light->is_active) return 0.0f;
    if (light->flicker_amount <= 0.0f) return light->intensity;
    float noise = (sinf(light->flicker_timer) + 0.5f * sinf(light->flicker_timer * 2.7f)) * 0.5f;
    float factor = 1.0f + noise * light->flicker_amount;
    if (factor < 0.0f) factor = 0.0f;
    return light->intensity * factor;
}
