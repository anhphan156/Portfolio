#include "application.h"
#include "asset/asset_manager.h"
#include "constant.h"
#include "scene/game_scene.h"
#include <raylib.h>
#include <stdlib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

AssetManager *am;

void app_init() {
    InitWindow(WIDTH, HEIGHT, "Web");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    am = malloc(sizeof(AssetManager));
    am_load_assets(am);

    game_scene_init(am->assets);
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
    am_destroy_assets(am);
    free(am);
    CloseWindow();
}
