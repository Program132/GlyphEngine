# Animation Module

The Animation module provides dynamic frame-by-frame animations for game objects in GlyphEngine. An animation consists of a sequence of frames, where each frame defines a display character (or Texture), foreground and background colors, and a frame duration in seconds. Animations can run as looping sequences (e.g. idle breathing, running, flickering fire) or one-shot sequences (e.g. sword slashes, explosions, death).

# Structs

### AnimationFrame

| Field     | Type            | Description                                                           |
|-----------|-----------------|-----------------------------------------------------------------------|
| character | char            | Glyph character displayed during this frame.                          |
| texture   | struct Texture* | Optional Texture pointer for multi-character frames.                  |
| fg        | Color           | Foreground color for this frame.                                      |
| bg        | Color           | Background color for this frame.                                      |
| duration  | float           | Time in seconds this frame remains active before advancing.           |

### Animation

| Field         | Type                                     | Description                                                       |
|---------------|------------------------------------------|-------------------------------------------------------------------|
| frames        | struct AnimationFrame[16]                | Fixed array holding up to 16 distinct animation frames.           |
| frame_count   | int                                      | Total number of frames registered in the sequence.                |
| current_frame | int                                      | Index of the currently active frame (0 to rame_count - 1).     |
| elapsed_time  | float                                    | Time accumulated within the current frame.                        |
| is_looping    | int                                      | 1 if the animation restarts automatically, 0 if it stops at end.  |
| is_playing    | int                                      | 1 if the animation advances over time, 0 if paused or stopped.   |
| is_finished   | int                                      | 1 if a non-looping animation reached the final frame.             |

# Functions

| Name                            | Return Type       | Arguments                                                            | Description                                                                     |
|---------------------------------|-------------------|----------------------------------------------------------------------|---------------------------------------------------------------------------------|
| animation_new                   | struct Animation* | int is_looping                                                       | Dynamically allocates and initializes a new animation instance.                 |
| animation_build                 | void              | struct Animation *anim, int is_looping                               | Initializes fields for an existing Animation structure.                         |
| animation_free                  | void              | struct Animation *anim                                               | Frees the dynamically allocated memory for the animation.                       |
| animation_add_frame_char        | int               | struct Animation *anim, char ch, Color fg, Color bg, float duration  | Appends a single-character frame with custom colors and duration (returns 1).   |
| animation_add_frame_texture     | int               | struct Animation *anim, struct Texture *tex, Color fg, Color bg, float dur | Appends a textured frame with custom colors and duration (returns 1).      |
| animation_update                | void              | struct Animation *anim, float dt                                     | Advances the animation timeline by delta time, stepping frames as needed.       |
| animation_play                  | void              | struct Animation *anim                                               | Resumes or starts playback.                                                     |
| animation_pause                 | void              | struct Animation *anim                                               | Pauses playback without resetting the current frame.                            |
| animation_stop                  | void              | struct Animation *anim                                               | Stops playback and rewinds to frame 0.                                          |
| animation_reset                 | void              | struct Animation *anim                                               | Rewinds to frame 0 and restarts playback immediately.                           |
| animation_get_current_character | char              | struct Animation *anim                                               | Returns the active frame character.                                             |
| animation_get_current_texture   | struct Texture*   | struct Animation *anim                                               | Returns the active frame texture (or NULL if character-based).                  |
| animation_get_current_fg        | Color             | struct Animation *anim                                               | Returns the active frame foreground color.                                      |
| animation_get_current_bg        | Color             | struct Animation *anim                                               | Returns the active frame background color.                                      |
| animation_is_finished           | int               | struct Animation *anim                                               | Returns 1 if a one-shot animation has reached its conclusion.                   |

# Usage Example

`c
#include  animation/animation.h
#include gameobject/player/player.h

// 1. Create a looping walk cycle
struct Animation *anim_walk = animation_new(1);
animation_add_frame_char(anim_walk, '@', COLOR_BRIGHT_WHITE, COLOR_DEFAULT, 0.12f);
animation_add_frame_char(anim_walk, 'A', COLOR_BRIGHT_WHITE, COLOR_DEFAULT, 0.12f);

// 2. In your update loop
animation_update(anim_walk, dt);
player_set_character(player, animation_get_current_character(anim_walk));
player_set_color(player, animation_get_current_fg(anim_walk), animation_get_current_bg(anim_walk));
`
