#ifndef COLOR_H
#define COLOR_H

typedef enum {
    COLOR_DEFAULT = 0,
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
    COLOR_BRIGHT_BLACK,
    COLOR_BRIGHT_RED,
    COLOR_BRIGHT_GREEN,
    COLOR_BRIGHT_YELLOW,
    COLOR_BRIGHT_BLUE,
    COLOR_BRIGHT_MAGENTA,
    COLOR_BRIGHT_CYAN,
    COLOR_BRIGHT_WHITE
} Color;

const char* color_to_ansi_fg(Color color);
const char* color_to_ansi_bg(Color color);
const char* color_reset_ansi(void);

#define COLOR_RGB_FLAG 0x01000000U

#define COLOR_RGB_ORANGE  0x01FFA500U
#define COLOR_RGB_GOLD    0x01FFD700U
#define COLOR_RGB_PURPLE  0x01800080U
#define COLOR_RGB_CRIMSON 0x01DC143CU
#define COLOR_RGB_LIME    0x0100FF00U
#define COLOR_RGB_AMBER   0x01FFBF00U
#define COLOR_RGB_TEAL    0x01008080U
#define COLOR_RGB_NAVY    0x01000080U
#define COLOR_RGB_INDIGO  0x014B0082U
#define COLOR_RGB_PINK    0x01FFC0CBU
#define COLOR_RGB_CORAL   0x01FF7F50U
#define COLOR_RGB_BROWN   0x018B4513U

Color color_rgb(unsigned char r, unsigned char g, unsigned char b);
Color color_hex(unsigned int hex);
int color_is_rgb(Color color);
Color color_ansi_to_rgb(Color color);
unsigned char color_get_r(Color color);
unsigned char color_get_g(Color color);
unsigned char color_get_b(Color color);
Color color_lerp(Color c1, Color c2, float t);
Color color_brightness(Color c, float factor);

#endif
