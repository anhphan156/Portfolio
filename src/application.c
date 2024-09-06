#include "application.h"
#include "constant.h"
#include "scene/game_scene.h"
#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

void app_init() {
    InitWindow(WIDTH, HEIGHT, "Web");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    game_scene_init();
}

void app_run() {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(game_scene_update, 0, 1);
#else
    while (!WindowShouldClose()) {
        game_scene_update();
    }
#endif
}

void app_destroy() {
    game_scene_destroy();
    CloseWindow();
}
