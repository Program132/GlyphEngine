#include "utils.h"
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#endif

static int vtp_initialized = 0;

static void ensure_vtp(void) {
#ifdef _WIN32
    if (!vtp_initialized) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        vtp_initialized = 1;
    }
#endif
}

void clearConsoleScreen(void) {
    ensure_vtp();
    printf("\033[H");
    fflush(stdout);
}

void hideCursor(void) {
    ensure_vtp();
    printf("\033[?25l");
    fflush(stdout);
}

void showCursor(void) {
    ensure_vtp();
    printf("\033[?25h");
    fflush(stdout);
}
