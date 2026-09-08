#ifndef LIGHT_H
#define LIGHT_H

#include "../color/color.h"

struct Light {
    float x;
    float y;
    float radius;
    float intensity;
    Color color;
    float flicker_amount;
    float flicker_speed;
    float flicker_timer;
    int is_active;
    int cast_shadows;
};

struct Light* light_new(float x, float y, float radius, float intensity, Color color);
void light_build(struct Light *light, float x, float y, float radius, float intensity, Color color);
void light_free(struct Light *light);

void light_set_position(struct Light *light, float x, float y);
void light_set_radius(struct Light *light, float radius);
void light_set_intensity(struct Light *light, float intensity);
void light_set_color(struct Light *light, Color color);
void light_set_flicker(struct Light *light, float amount, float speed);
void light_set_cast_shadows(struct Light *light, int cast_shadows);
void light_set_active(struct Light *light, int active);

void light_update(struct Light *light, float dt);
float light_get_current_intensity(struct Light *light);

#endif
