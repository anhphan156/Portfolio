#include "scene/game_scene.h"
#include "constant.h"
#include "ds/linkedlist.h"
#include "raylib.h"
#include "scene/scene.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void s_render();
void s_spawn();
void s_collision_detection();
void s_integrate(float dt);
void s_render_grid();

Texture2D *textures;
GameScene *game_scene;
float      cd     = 0.0;
float      offset = 0.0;

void game_scene_update() {
    offset += .005;

    s_spawn();
    em_update(&game_scene->em);
    s_collision_detection();
    s_integrate(GetFrameTime());

    BeginDrawing();

    ClearBackground(BLACK);
    s_render_grid();

    BeginMode2D(game_scene->camera);

    Rectangle source;
    source.x      = 0.0;
    source.y      = 0.0;
    source.width  = 64.0;
    source.height = 64.0;
    Rectangle dest;
    dest.x      = 0.0;
    dest.y      = 64.0 * 15;
    dest.width  = 128.0;
    dest.height = 128.0;
    Vector2 origin;
    origin.x = 0.0;
    origin.y = 0.0;
    DrawTexturePro(textures[14], source, dest, origin, 0.0, WHITE);

    s_render();

    EndMode2D();

    DrawFPS(0, 0);

    EndDrawing();
}

GameScene *game_scene_init(Texture2D *t) {
    textures = t;

    game_scene = malloc(sizeof(GameScene));

    game_scene->scene_type = GAME_SCENE;

    game_scene->em.pending.head    = 0;
    game_scene->em.pending.length  = 0;
    game_scene->em.entities.head   = 0;
    game_scene->em.entities.length = 0;

    game_scene->camera.offset   = (Vector2){0.0, 0.0};
    game_scene->camera.target   = (Vector2){0.0, 0.0};
    game_scene->camera.rotation = 0.0;
    game_scene->camera.zoom     = 1.0;

    return game_scene;
}

void game_scene_destroy() {
    em_destroy(&game_scene->em);
}

void s_collision_detection() {
    for (Node *i = game_scene->em.entities.head; i != 0; i = i->next) {
        Entity *e = (Entity *)i->data;
        if (e == 0)
            continue;

        if (e->is_alive == 0)
            continue;

        Vector2 pos = e->transform.position;

        if (pos.x <= 0.0 || pos.x > WIDTH || pos.y < 0.0 || pos.y > HEIGHT) {
            e->is_alive = 0;
        }
    }
}

void s_spawn() {

    if (cd > 0) {
        cd -= GetFrameTime();
        return;
    }
    cd = 0.5;

    int   count = 10;
    float a     = 6.28 / (float)count;
    float cur_a = 0.0;
    for (int i = 0; i < 10; i += 1) {
        Entity *e               = em_create(&game_scene->em);
        e->is_alive             = 1;
        e->transform.position.x = cos(cur_a + offset) + WIDTH / 2.0;
        e->transform.position.y = sin(cur_a + offset) + HEIGHT / 2.0;
        e->transform.velocity.x = cos(cur_a + offset) * 200.0;
        e->transform.velocity.y = sin(cur_a + offset) * 200.0;
        cur_a += a;
    }
}

void s_render() {
    for (Node *i = game_scene->em.entities.head; i != 0; i = i->next) {
        Entity *e = (Entity *)i->data;
        if (e == 0)
            continue;

        if (e->is_alive == 0)
            continue;

        Vector2 pos = e->transform.position;

        DrawCircle(pos.x, pos.y, 30.0, RED);
    }
}

void s_integrate(float dt) {
    for (Node *i = game_scene->em.entities.head; i != 0; i = i->next) {
        Entity *e = (Entity *)i->data;
        if (e == 0)
            continue;

        if (e->is_alive == 0)
            continue;

        Vector2  vec = e->transform.velocity;
        Vector2 *pos = &e->transform.position;

        pos->x += vec.x * dt;
        pos->y += vec.y * dt;
    }
}

void s_render_grid() {
    int cell_size = 64;
    int count_x   = WIDTH / cell_size;
    int count_y   = HEIGHT / cell_size;

    for (int i = 1; i <= count_x; i += 1) {
        DrawLine(i * cell_size, 0, i * cell_size, HEIGHT, RED);
    }

    for (int i = 1; i <= count_y; i += 1) {
        DrawLine(0, i * cell_size, WIDTH, i * cell_size, RED);
    }
}
