#include "scene/game_scene.h"
#include "component/ctexture.h"
#include "constant.h"
#include "ds/linkedlist.h"
#include "entity/entity_manager.h"
#include "level/level_parser.h"
#include "raylib.h"
#include "scene/scene.h"
#include <math.h>
#include <stdlib.h>

void s_render();
void s_spawn();
void s_collision_detection();
void s_integrate(float dt);
void s_render_grid();

Texture2D *textures;
GameScene *game_scene;
float      cd        = 0.0;
float      offset    = 0.0;
int        cell_size = 128;

void game_scene_update() {
    offset += .005;

    s_spawn();
    em_update(&game_scene->em);
    s_integrate(GetFrameTime());

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(game_scene->camera);

    s_render_grid();
    s_render();

    EndMode2D();
    DrawFPS(0, 0);
    EndDrawing();
}

GameScene *game_scene_init(Texture2D *t) {
    textures = t;

    game_scene             = malloc(sizeof(GameScene));
    game_scene->scene_type = GAME_SCENE;
    em_init(&game_scene->em);

    game_scene->camera.offset   = (Vector2){0.0, 0.0};
    game_scene->camera.target   = (Vector2){0.0, 0.0};
    game_scene->camera.rotation = 0.0;
    game_scene->camera.zoom     = 1.0;

    level_parser("gamedata/level.data", &game_scene->em, &cell_size);

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
    return;

    if (cd > 0) {
        cd -= GetFrameTime();
        return;
    }
    cd = 0.5;

    int   count = 10;
    float a     = 6.28 / (float)count;
    float cur_a = 0.0;
    for (int i = 0; i < 10; i += 1) {
        Entity *e               = em_create_entity(&game_scene->em);
        e->is_alive             = 1;
        e->transform.position.x = cos(cur_a + offset) + WIDTH / 2.0;
        e->transform.position.y = sin(cur_a + offset) + HEIGHT / 2.0;
        e->transform.velocity.x = cos(cur_a + offset) * 200.0;
        e->transform.velocity.y = sin(cur_a + offset) * 200.0;
        e->texture.id           = 14;
        e->shape.r              = 64.0;
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

        Vector2 pos        = e->transform.position;
        int     texture_id = e->texture.id;
        float   shape_size = e->shape.r;

        if (texture_id == NO_TEXTURE) {
            DrawCircle(pos.x, pos.y, shape_size, RED);
        } else {
            Rectangle source;
            source.x      = 0.0;
            source.y      = 0.0;
            source.width  = textures[texture_id].width;
            source.height = textures[texture_id].height;
            Rectangle dest;
            dest.x      = pos.x;
            dest.y      = pos.y;
            dest.width  = shape_size;
            dest.height = shape_size;
            Vector2 origin;
            origin.x = 0.0;
            origin.y = 0.0;
            DrawTexturePro(textures[texture_id], source, dest, origin, 0.0, WHITE);
        }
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
    int count_x = WIDTH / cell_size;
    int count_y = HEIGHT / cell_size;
    int y_offet = HEIGHT % cell_size;

    for (int i = 1; i < count_x; i += 1) {
        DrawLine(i * cell_size, 0, i * cell_size, HEIGHT, RED);
    }

    for (int i = 0; i <= count_y; i += 1) {
        DrawLine(0, i * cell_size - y_offet, WIDTH, i * cell_size - y_offet, RED);
    }
}
