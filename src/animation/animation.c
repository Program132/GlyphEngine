#include "animation.h"
#include <stdlib.h>

struct Animation* animation_new(int is_looping) {
    struct Animation *anim = (struct Animation*)malloc(sizeof(struct Animation));
    if (anim == NULL) return NULL;
    animation_build(anim, is_looping);
    return anim;
}

void animation_build(struct Animation *anim, int is_looping) {
    if (anim == NULL) return;
    anim->frame_count = 0;
    anim->current_frame = 0;
    anim->elapsed_time = 0.0f;
    anim->is_looping = is_looping;
    anim->is_playing = 1;
    anim->is_finished = 0;
    for (int i = 0; i < MAX_ANIMATION_FRAMES; i++) {
        anim->frames[i].character = '\0';
        anim->frames[i].texture = NULL;
        anim->frames[i].fg = COLOR_DEFAULT;
        anim->frames[i].bg = COLOR_DEFAULT;
        anim->frames[i].duration = 0.1f;
    }
}

void animation_free(struct Animation *anim) {
    if (anim != NULL) {
        free(anim);
    }
}

int animation_add_frame_char(struct Animation *anim, char character, Color fg, Color bg, float duration) {
    if (anim == NULL || anim->frame_count >= MAX_ANIMATION_FRAMES || duration <= 0.0f) return 0;
    int idx = anim->frame_count;
    anim->frames[idx].character = character;
    anim->frames[idx].texture = NULL;
    anim->frames[idx].fg = fg;
    anim->frames[idx].bg = bg;
    anim->frames[idx].duration = duration;
    anim->frame_count++;
    return 1;
}

int animation_add_frame_texture(struct Animation *anim, struct Texture *texture, Color fg, Color bg, float duration) {
    if (anim == NULL || anim->frame_count >= MAX_ANIMATION_FRAMES || duration <= 0.0f) return 0;
    int idx = anim->frame_count;
    anim->frames[idx].character = '\0';
    anim->frames[idx].texture = texture;
    anim->frames[idx].fg = fg;
    anim->frames[idx].bg = bg;
    anim->frames[idx].duration = duration;
    anim->frame_count++;
    return 1;
}

void animation_update(struct Animation *anim, float dt) {
    if (anim == NULL || !anim->is_playing || anim->frame_count == 0 || anim->is_finished) return;

    anim->elapsed_time += dt;
    while (anim->elapsed_time >= anim->frames[anim->current_frame].duration) {
        anim->elapsed_time -= anim->frames[anim->current_frame].duration;
        if (anim->current_frame + 1 < anim->frame_count) {
            anim->current_frame++;
        } else {
            if (anim->is_looping) {
                anim->current_frame = 0;
            } else {
                anim->is_finished = 1;
                anim->is_playing = 0;
                break;
            }
        }
    }
}

void animation_play(struct Animation *anim) {
    if (anim == NULL) return;
    anim->is_playing = 1;
}

void animation_pause(struct Animation *anim) {
    if (anim == NULL) return;
    anim->is_playing = 0;
}

void animation_stop(struct Animation *anim) {
    if (anim == NULL) return;
    anim->is_playing = 0;
    anim->current_frame = 0;
    anim->elapsed_time = 0.0f;
    anim->is_finished = 0;
}

void animation_reset(struct Animation *anim) {
    if (anim == NULL) return;
    anim->current_frame = 0;
    anim->elapsed_time = 0.0f;
    anim->is_finished = 0;
    anim->is_playing = 1;
}

char animation_get_current_character(struct Animation *anim) {
    if (anim == NULL || anim->frame_count == 0) return '\0';
    return anim->frames[anim->current_frame].character;
}

struct Texture* animation_get_current_texture(struct Animation *anim) {
    if (anim == NULL || anim->frame_count == 0) return NULL;
    return anim->frames[anim->current_frame].texture;
}

Color animation_get_current_fg(struct Animation *anim) {
    if (anim == NULL || anim->frame_count == 0) return COLOR_DEFAULT;
    return anim->frames[anim->current_frame].fg;
}

Color animation_get_current_bg(struct Animation *anim) {
    if (anim == NULL || anim->frame_count == 0) return COLOR_DEFAULT;
    return anim->frames[anim->current_frame].bg;
}

int animation_is_finished(struct Animation *anim) {
    if (anim == NULL) return 1;
    return anim->is_finished;
}
