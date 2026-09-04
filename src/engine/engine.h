#ifndef ENGINE_H
#define ENGINE_H

#include "../level/level.h"

#ifdef _WIN32
    #include <windows.h>
    #define SLEEP_MS(ms) Sleep(ms)
#elif defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    #include <sys/time.h>
    #define SLEEP_MS(ms) usleep((ms) * 1000)
#else
    #error "Plateforme non supportée"
#endif

struct Engine {
    struct Level *current_level;
    int width;
    int height;
    int is_running;
    void (*on_update)(struct Engine* engine, float delta_time);
};

struct Engine* engine_new(struct Level *current_level, int width, int height);
void engine_build(struct Engine* engine, struct Level *current_level, int width, int height);
void engine_free(struct Engine* engine);

void engine_run(struct Engine* engine, int fps);
void engine_stop(struct Engine* engine);

void engine_set_level(struct Engine* engine, struct Level *new_level);
void engine_set_update_callback(struct Engine* engine, void (*update_func)(struct Engine*, float));

#endif