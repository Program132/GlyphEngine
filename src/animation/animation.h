#ifndef ANIMATION_H
#define ANIMATION_H

#include "../texture/texture.h"
#include "../color/color.h"

#define MAX_ANIMATION_FRAMES 16

struct AnimationFrame {
    char character;
    struct Texture *texture;
    Color fg;
    Color bg;
    float duration;
};

struct Animation {
    struct AnimationFrame frames[MAX_ANIMATION_FRAMES];
    int frame_count;
    int current_frame;
    float elapsed_time;
    int is_looping;
    int is_playing;
    int is_finished;
};

struct Animation* animation_new(int is_looping);
void animation_build(struct Animation *anim, int is_looping);
void animation_free(struct Animation *anim);

int animation_add_frame_char(struct Animation *anim, char character, Color fg, Color bg, float duration);
int animation_add_frame_texture(struct Animation *anim, struct Texture *texture, Color fg, Color bg, float duration);

void animation_update(struct Animation *anim, float dt);
void animation_play(struct Animation *anim);
void animation_pause(struct Animation *anim);
void animation_stop(struct Animation *anim);
void animation_reset(struct Animation *anim);

char animation_get_current_character(struct Animation *anim);
struct Texture* animation_get_current_texture(struct Animation *anim);
Color animation_get_current_fg(struct Animation *anim);
Color animation_get_current_bg(struct Animation *anim);
int animation_is_finished(struct Animation *anim);

#endif
