#include "engine.h"
#include <stdlib.h>
#include <stdio.h>
#include "../inputs/inputs.h"
#include "../utils/utils.h"
#include "../levelui/levelui.h"

struct Engine* engine_new(struct Level *current_level, int width, int height) {
    struct Engine* engine = (struct Engine*)malloc(sizeof(struct Engine));
    if (engine == NULL) return NULL;
    engine_build(engine, current_level, width, height);
    return engine;
}

void engine_build(struct Engine* engine, struct Level *current_level, int width, int height) {
    engine->current_level = current_level;
    engine->current_ui = NULL;
    engine->width = width;
    engine->height = height;
    engine->is_running = 0;
    engine->exit_on_escape = 1;
    engine->on_update = NULL;
}

void engine_free(struct Engine* engine) {
    if (engine != NULL) {
        if (engine->current_level != NULL) {
            level_free(engine->current_level);
        }
        if (engine->current_ui != NULL) {
            level_ui_free(engine->current_ui);
        }
        free(engine);
    }
}

void engine_stop(struct Engine* engine) {
    if (engine != NULL) {
        engine->is_running = 0;
    }
}

void engine_set_level(struct Engine* engine, struct Level *new_level) {
    if (engine != NULL) {
        engine->current_level = new_level;
        level_reset_screen_buffer();
        level_ui_reset_screen_buffer();
    }
}

void engine_set_level_ui(struct Engine* engine, struct LevelUI *ui) {
    if (engine != NULL) {
        engine->current_ui = ui;
        level_reset_screen_buffer();
        level_ui_reset_screen_buffer();
    }
}

struct LevelUI* engine_get_level_ui(struct Engine* engine) {
    if (engine == NULL) return NULL;
    return engine->current_ui;
}

void engine_set_exit_on_escape(struct Engine* engine, int enabled) {
    if (engine != NULL) {
        engine->exit_on_escape = enabled;
    }
}

int engine_get_exit_on_escape(struct Engine* engine) {
    if (engine == NULL) return 0;
    return engine->exit_on_escape;
}

void engine_set_update_callback(struct Engine* engine, void (*update_func)(struct Engine*, float)) {
    if (engine != NULL) {
        engine->on_update = update_func;
    }
}

static double get_time_in_seconds() {
#ifdef _WIN32
    LARGE_INTEGER frequency;
    LARGE_INTEGER time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&time);
    return (double)time.QuadPart / (double)frequency.QuadPart;
#elif defined(__linux__) || defined(__APPLE__)
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
#else
    return 0.0;
#endif
}

void engine_run(struct Engine* engine, int fps) {
    if (fps <= 0) fps = 30;
    if (engine == NULL) return;

    engine->is_running = 1;
    double target_frame_time = 1.0 / (double)fps;
    double last_time = get_time_in_seconds();

    hideCursor();

    while (engine->is_running) {
        if (engine->exit_on_escape && input_is_key_down(KEY_ESCAPE)) {
            engine_stop(engine);
            break;
        }

        double current_time = get_time_in_seconds();
        float delta_time = (float)(current_time - last_time);
        last_time = current_time;

        if (engine->on_update != NULL) {
            engine->on_update(engine, delta_time);
        }

        if (engine->is_running) {
            if (engine->current_ui != NULL) {
                level_ui_display(engine->current_ui);
            } else if (engine->current_level != NULL) {
                level_display(engine->current_level);
            }
        }

        double end_time = get_time_in_seconds();
        double frame_duration = end_time - current_time;
        double time_to_sleep = target_frame_time - frame_duration;

        if (time_to_sleep > 0.0) {
            SLEEP_MS((int)(time_to_sleep * 1000.0));
        }
    }

    showCursor();
    input_flush();
}