#include "Application.h"
#include <math.h>
#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

Camera2D camera;

void app_update_frame(void) {
    BeginDrawing();
    BeginMode2D(camera);

    ClearBackground(BLACK);

    DrawText("hello world", 380, 295, 30, RED);
    DrawCircle(380 + sin(GetTime()) * 200, 400, 50, BLUE);

    EndMode2D();
    EndDrawing();
}

void app_init() {
    InitWindow(1200, 800, "Web");
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    camera.offset   = (Vector2){0.0, 0.0};
    camera.target   = (Vector2){0.0, 0.0};
    camera.rotation = 0.0;
    camera.zoom     = 1.0;
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

void app_destroy() {
    CloseWindow();
}
