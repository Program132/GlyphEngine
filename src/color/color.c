#include "color.h"

const char* color_to_ansi_fg(Color color) {
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
