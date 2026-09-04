#include "inputs.h"
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#elif defined(__linux__) || defined(__APPLE__)
    #include <termios.h>
    #include <fcntl.h>
    #include <unistd.h>
#endif

#ifdef _WIN32
static int keycode_to_vk(KeyCode key) {
    if (key >= KEY_A && key <= KEY_Z) return key - 32;
    if (key >= KEY_A_UPPER && key <= KEY_Z_UPPER) return key;
    if (key >= KEY_0 && key <= KEY_9) return key;

    switch (key) {
        case KEY_ESCAPE:    return VK_ESCAPE;
        case KEY_ENTER:     return VK_RETURN;
        case KEY_RETURN:    return VK_RETURN;
        case KEY_TAB:       return VK_TAB;
        case KEY_BACKSPACE: return VK_BACK;
        case KEY_SPACE:     return VK_SPACE;
        case KEY_UP:        return VK_UP;
        case KEY_DOWN:      return VK_DOWN;
        case KEY_LEFT:      return VK_LEFT;
        case KEY_RIGHT:     return VK_RIGHT;
        case KEY_INSERT:    return VK_INSERT;
        case KEY_DELETE:    return VK_DELETE;
        case KEY_HOME:      return VK_HOME;
        case KEY_END:       return VK_END;
        case KEY_PAGE_UP:   return VK_PRIOR;
        case KEY_PAGE_DOWN: return VK_NEXT;
        case KEY_F1:        return VK_F1;
        case KEY_F2:        return VK_F2;
        case KEY_F3:        return VK_F3;
        case KEY_F4:        return VK_F4;
        case KEY_F5:        return VK_F5;
        case KEY_F6:        return VK_F6;
        case KEY_F7:        return VK_F7;
        case KEY_F8:        return VK_F8;
        case KEY_F9:        return VK_F9;
        case KEY_F10:       return VK_F10;
        case KEY_F11:       return VK_F11;
        case KEY_F12:       return VK_F12;
        default:            return 0;
    }
}
#endif

int input_is_key_down(KeyCode key) {
#ifdef _WIN32
    int vk = keycode_to_vk(key);
    if (vk == 0) return 0;
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
#elif defined(__linux__) || defined(__APPLE__)
    KeyCode current = input_get_key();
    if (current == key) return 1;
    if (key >= KEY_A && key <= KEY_Z && current == key - 32) return 1;
    if (key >= KEY_A_UPPER && key <= KEY_Z_UPPER && current == key + 32) return 1;
    return 0;
#else
    return 0;
#endif
}

KeyCode input_get_key(void) {
#ifdef _WIN32
    if (_kbhit()) {
        int ch = _getch();
        if (ch == 0 || ch == 224) {
            int ext = _getch();
            switch (ext) {
                case 72: return KEY_UP;
                case 80: return KEY_DOWN;
                case 75: return KEY_LEFT;
                case 77: return KEY_RIGHT;
                default: return KEY_UNKNOWN;
            }
        }
        return (KeyCode)ch;
    }
    return KEY_UNKNOWN;
#elif defined(__linux__) || defined(__APPLE__)
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == 27) {
        int next_ch = getchar();
        if (next_ch == '[') {
            int arrow = getchar();
            if (arrow == 'A') return KEY_UP;
            if (arrow == 'B') return KEY_DOWN;
            if (arrow == 'C') return KEY_RIGHT;
            if (arrow == 'D') return KEY_LEFT;
        }
        return KEY_ESCAPE;
    }

    if (ch != EOF) {
        return (KeyCode)ch;
    }
    return KEY_UNKNOWN;
#else
    return KEY_UNKNOWN;
#endif
}

void input_flush(void) {
#ifdef _WIN32
    while (_kbhit()) {
        _getch();
    }
#elif defined(__linux__) || defined(__APPLE__)
    while (input_get_key() != KEY_UNKNOWN) {}
#endif
}
