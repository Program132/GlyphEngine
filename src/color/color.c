#include "color.h"
#include <stdio.h>

static char s_fg_buffers[16][32];
static int s_fg_idx = 0;

static char s_bg_buffers[16][32];
static int s_bg_idx = 0;

const char* color_to_ansi_fg(Color color) {
    if (color_is_rgb(color)) {
        char *buf = s_fg_buffers[s_fg_idx++ & 15];
        snprintf(buf, sizeof(s_fg_buffers[0]), "\033[38;2;%u;%u;%um",
                 (unsigned int)((color >> 16) & 0xFF),
                 (unsigned int)((color >> 8) & 0xFF),
                 (unsigned int)(color & 0xFF));
        return buf;
    }
    switch (color) {
        case COLOR_BLACK: return "\033[30m";
        case COLOR_RED: return "\033[31m";
        case COLOR_GREEN: return "\033[32m";
        case COLOR_YELLOW: return "\033[33m";
        case COLOR_BLUE: return "\033[34m";
        case COLOR_MAGENTA: return "\033[35m";
        case COLOR_CYAN: return "\033[36m";
        case COLOR_WHITE: return "\033[37m";
        case COLOR_BRIGHT_BLACK: return "\033[90m";
        case COLOR_BRIGHT_RED: return "\033[91m";
        case COLOR_BRIGHT_GREEN: return "\033[92m";
        case COLOR_BRIGHT_YELLOW: return "\033[93m";
        case COLOR_BRIGHT_BLUE: return "\033[94m";
        case COLOR_BRIGHT_MAGENTA: return "\033[95m";
        case COLOR_BRIGHT_CYAN: return "\033[96m";
        case COLOR_BRIGHT_WHITE: return "\033[97m";
        default: return "\033[39m";
    }
}

const char* color_to_ansi_bg(Color color) {
    if (color_is_rgb(color)) {
        char *buf = s_bg_buffers[s_bg_idx++ & 15];
        snprintf(buf, sizeof(s_bg_buffers[0]), "\033[48;2;%u;%u;%um",
                 (unsigned int)((color >> 16) & 0xFF),
                 (unsigned int)((color >> 8) & 0xFF),
                 (unsigned int)(color & 0xFF));
        return buf;
    }
    switch (color) {
        case COLOR_BLACK: return "\033[40m";
        case COLOR_RED: return "\033[41m";
        case COLOR_GREEN: return "\033[42m";
        case COLOR_YELLOW: return "\033[43m";
        case COLOR_BLUE: return "\033[44m";
        case COLOR_MAGENTA: return "\033[45m";
        case COLOR_CYAN: return "\033[46m";
        case COLOR_WHITE: return "\033[47m";
        case COLOR_BRIGHT_BLACK: return "\033[100m";
        case COLOR_BRIGHT_RED: return "\033[101m";
        case COLOR_BRIGHT_GREEN: return "\033[102m";
        case COLOR_BRIGHT_YELLOW: return "\033[103m";
        case COLOR_BRIGHT_BLUE: return "\033[104m";
        case COLOR_BRIGHT_MAGENTA: return "\033[105m";
        case COLOR_BRIGHT_CYAN: return "\033[106m";
        case COLOR_BRIGHT_WHITE: return "\033[107m";
        default: return "\033[49m";
    }
}

const char* color_reset_ansi(void) {
    return "\033[0m";
}

Color color_rgb(unsigned char r, unsigned char g, unsigned char b) {
    return (Color)(COLOR_RGB_FLAG | ((unsigned int)r << 16) | ((unsigned int)g << 8) | (unsigned int)b);
}

Color color_hex(unsigned int hex) {
    return color_rgb((unsigned char)((hex >> 16) & 0xFF),
                     (unsigned char)((hex >> 8) & 0xFF),
                     (unsigned char)(hex & 0xFF));
}

int color_is_rgb(Color color) {
    return ((unsigned int)color & COLOR_RGB_FLAG) == COLOR_RGB_FLAG;
}

Color color_ansi_to_rgb(Color color) {
    if (color_is_rgb(color)) return color;
    switch (color) {
        case COLOR_BLACK: return color_rgb(0, 0, 0);
        case COLOR_RED: return color_rgb(170, 0, 0);
        case COLOR_GREEN: return color_rgb(0, 170, 0);
        case COLOR_YELLOW: return color_rgb(170, 85, 0);
        case COLOR_BLUE: return color_rgb(0, 0, 170);
        case COLOR_MAGENTA: return color_rgb(170, 0, 170);
        case COLOR_CYAN: return color_rgb(0, 170, 170);
        case COLOR_WHITE: return color_rgb(170, 170, 170);
        case COLOR_BRIGHT_BLACK: return color_rgb(85, 85, 85);
        case COLOR_BRIGHT_RED: return color_rgb(255, 85, 85);
        case COLOR_BRIGHT_GREEN: return color_rgb(85, 255, 85);
        case COLOR_BRIGHT_YELLOW: return color_rgb(255, 255, 85);
        case COLOR_BRIGHT_BLUE: return color_rgb(85, 85, 255);
        case COLOR_BRIGHT_MAGENTA: return color_rgb(255, 85, 255);
        case COLOR_BRIGHT_CYAN: return color_rgb(85, 255, 255);
        case COLOR_BRIGHT_WHITE: return color_rgb(255, 255, 255);
        default: return color_rgb(255, 255, 255);
    }
}

unsigned char color_get_r(Color color) {
    if (color_is_rgb(color)) return (unsigned char)((color >> 16) & 0xFF);
    Color rgb = color_ansi_to_rgb(color);
    return (unsigned char)((rgb >> 16) & 0xFF);
}

unsigned char color_get_g(Color color) {
    if (color_is_rgb(color)) return (unsigned char)((color >> 8) & 0xFF);
    Color rgb = color_ansi_to_rgb(color);
    return (unsigned char)((rgb >> 8) & 0xFF);
}

unsigned char color_get_b(Color color) {
    if (color_is_rgb(color)) return (unsigned char)(color & 0xFF);
    Color rgb = color_ansi_to_rgb(color);
    return (unsigned char)(rgb & 0xFF);
}

Color color_lerp(Color c1, Color c2, float t) {
    if (t <= 0.0f) return c1;
    if (t >= 1.0f) return c2;
    unsigned char r1 = color_get_r(c1);
    unsigned char g1 = color_get_g(c1);
    unsigned char b1 = color_get_b(c1);
    unsigned char r2 = color_get_r(c2);
    unsigned char g2 = color_get_g(c2);
    unsigned char b2 = color_get_b(c2);
    int r = (int)((float)r1 + ((float)r2 - (float)r1) * t + 0.5f);
    int g = (int)((float)g1 + ((float)g2 - (float)g1) * t + 0.5f);
    int b = (int)((float)b1 + ((float)b2 - (float)b1) * t + 0.5f);
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;
    return color_rgb((unsigned char)r, (unsigned char)g, (unsigned char)b);
}

Color color_brightness(Color c, float factor) {
    if (factor <= 0.0f) return color_rgb(0, 0, 0);
    int r = (int)((float)color_get_r(c) * factor + 0.5f);
    int g = (int)((float)color_get_g(c) * factor + 0.5f);
    int b = (int)((float)color_get_b(c) * factor + 0.5f);
    if (r < 0) r = 0; if (r > 255) r = 255;
    if (g < 0) g = 0; if (g > 255) g = 255;
    if (b < 0) b = 0; if (b > 255) b = 255;
    return color_rgb((unsigned char)r, (unsigned char)g, (unsigned char)b);
}
