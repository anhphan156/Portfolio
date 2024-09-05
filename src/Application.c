#include "Application.h"
#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif

void app_init() {
    InitWindow(1200, 800, "Web");
    SetTargetFPS(60);
}

void app_run() {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(app_update_frame, 0, 1);
#else
    while (!WindowShouldClose()) {
        app_update_frame();
    }
#endif
}

void app_update_frame(void) {
    BeginDrawing();
    ClearBackground(GREEN);
    DrawText("hello world", 380, 295, 30, RED);
    EndDrawing();
}

void app_destroy() {
    CloseWindow();
}
