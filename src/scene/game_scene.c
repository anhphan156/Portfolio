#include "scene/game_scene.h"
#include "component/cbbox.h"
#include "component/ctexture.h"
#include "constant.h"
#include "ds/linkedlist.h"
#include "entity/entity_manager.h"
#include "level/level_parser.h"
#include "physics/aabb_detection.h"
#include "raylib.h"
#include "scene/scene.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void s_collision_detection();
void s_collision_resolution();
void s_player_movement();
void s_integrate(float dt);
void s_input();
void s_render();
void s_render_grid();
void s_render_bbox();

Texture2D *textures;
GameScene *game_scene;
Entity    *player;
int        cell_size     = 128;
int        physics_debug = 0;

void game_scene_update() {
    em_update(&game_scene->em);
    s_input();
    s_player_movement();
    s_integrate(GetFrameTime());
    s_collision_detection();
    s_collision_resolution();

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(game_scene->camera);

    if (physics_debug) {
        s_render_grid();
        s_render_bbox();
    } else {
        s_render();
    }

    EndMode2D();
    DrawFPS(0, 0);
    EndDrawing();
}

GameScene *game_scene_init(Texture2D *t) {
    textures = t;

    game_scene             = malloc(sizeof(GameScene));
    game_scene->scene_type = GAME_SCENE;
    em_init(&game_scene->em);

    // camera setup
    game_scene->camera.offset   = (Vector2){0.0, 0.0};
    game_scene->camera.target   = (Vector2){0.0, 0.0};
    game_scene->camera.rotation = 0.0;
    game_scene->camera.zoom     = 1.0;

    // spawn level
    level_parser("gamedata/level.data", &game_scene->em, &cell_size);

    // spawn player
    player                       = em_create_entity(&game_scene->em);
    player->transform.position.x = 300.0;
    player->transform.position.y = 300.0;
    player->transform.velocity.x = 0.0;
    player->transform.velocity.y = 300.0;
    player->texture.id           = 0;
    player->shape.half_box       = (Vector2){cell_size / 2.0, cell_size / 2.0};
    player->bbox.half_box        = (Vector2){cell_size / 2.0, cell_size / 2.0};
    player->bbox.enabled         = 1;

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

        if (e->is_alive == 0 || e->bbox.enabled == 0 || e == player)
            continue;

        Vector2 collider_pos      = e->transform.position;
        Vector2 collider_half_box = e->bbox.half_box;

        AABBResult *result          = &player->bbox.collision_result;
        Vector2     player_pos      = player->transform.position;
        Vector2     player_half_box = player->bbox.half_box;

        int collision = AABB_detection(player_pos, player_half_box, collider_pos, collider_half_box, &result->collision_axes, &result->overlapped_shape);
        if (collision == 1.0) {
            Vector2 player_prev_pos = player->transform.prev_position;
            AABB_detection(player_prev_pos, player_half_box, collider_pos, collider_half_box, &result->prev_collision_axes, NULL);

            return;
        }
    }

    player->state.on_ground = 0;

    player->bbox.collision_result.collision_axes      = (Vector2){0.0, 0.0};
    player->bbox.collision_result.prev_collision_axes = (Vector2){0.0, 0.0};
}

void s_collision_resolution() {
    if (!player)
        return;

    Vector2 collision_axes = player->bbox.collision_result.collision_axes;

    Vector2   *player_pos       = &player->transform.position;
    Vector2   *player_vec       = &player->transform.velocity;
    AABBResult collision_result = player->bbox.collision_result;

    Vector2 player_vec_dir;
    player_vec_dir.x = player_vec->x > 0.0 ? 1.0 : -1.0;
    player_vec_dir.y = player_vec->y > 0.0 ? 1.0 : -1.0;

    if (collision_axes.x * collision_axes.y == 1.0) {
        player->state.on_ground = 1;

        if (collision_result.prev_collision_axes.x == 1.0) {
            player_pos->y -= collision_result.overlapped_shape.y * player_vec_dir.y;

            if (player_vec_dir.y == -1.0) {
                // player falling down when hitting their head
                player_vec->y *= -0.8;
            }

        } else if (collision_result.prev_collision_axes.y == 1.0) {
            player_pos->x -= collision_result.overlapped_shape.x * player_vec_dir.x;
        }
    }
}

void s_player_movement() {
    char     input = player->input.input;
    Vector2 *vec   = &player->transform.velocity;
    if (input & (1 << LEFT_KEY_BIT)) {
        vec->x = -200.0;
    } else if (input & (1 << RIGHT_KEY_BIT)) {
        vec->x = 200.0;
    } else {
        vec->x = 0.0;
    }

    if (input & (1 << UP_KEY_BIT) && player->state.on_ground == 1) {
        vec->y = -1500.0;
    } else {
        vec->y += 15.0;
        vec->y = fmin(700.0, vec->y);
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

        e->transform.prev_position = *pos;

        pos->x += vec.x * dt;
        pos->y += vec.y * dt;
    }
}

void s_input() {
    if (IsKeyPressed(KEY_ONE)) {
        physics_debug ^= 1;
    }
    if (IsKeyPressed(KEY_TWO)) {
        player->transform.position = (Vector2){300.0, 300.0};
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        player->input.input |= 1 << LEFT_KEY_BIT;
    } else {
        player->input.input &= ~(1 << LEFT_KEY_BIT);
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        player->input.input |= 1 << RIGHT_KEY_BIT;
    } else {
        player->input.input &= ~(1 << RIGHT_KEY_BIT);
    }
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
        player->input.input |= (1 << UP_KEY_BIT);
    } else {
        player->input.input &= ~(1 << UP_KEY_BIT);
    }
}

void s_render() {
    for (Node *i = game_scene->em.entities.head; i != 0; i = i->next) {
        Entity *e = (Entity *)i->data;
        if (e == 0)
            continue;

        if (e->is_alive == 0)
            continue;

        Vector2 pos          = e->transform.position;
        int     texture_id   = e->texture.id;
        float   shape_size_w = e->shape.half_box.x;
        float   shape_size_h = e->shape.half_box.y;

        if (texture_id == NO_TEXTURE) {
            DrawCircle(pos.x, pos.y, shape_size_w, RED);
        } else {
            Rectangle source;
            source.x      = 0.0;
            source.y      = 0.0;
            source.width  = textures[texture_id].width;
            source.height = textures[texture_id].height;
            Rectangle dest;
            dest.x      = pos.x;
            dest.y      = pos.y;
            dest.width  = shape_size_w * 2.0;
            dest.height = shape_size_h * 2.0;
            DrawTexturePro(textures[texture_id], source, dest, e->shape.half_box, 0.0, WHITE);
        }
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

void s_render_bbox() {
    for (Node *i = game_scene->em.entities.head; i != 0; i = i->next) {
        Entity *e = i->data;
        if (e == 0)
            continue;
        if (e->is_alive == 0 || e->bbox.enabled == 0)
            continue;

        Vector2 pos      = e->transform.position;
        Vector2 half_box = e->bbox.half_box;

        Rectangle rec;
        rec.x      = pos.x;
        rec.y      = pos.y;
        rec.width  = half_box.x * 2.0;
        rec.height = half_box.y * 2.0;

        DrawRectanglePro(rec, half_box, 0.0, RED);
        DrawCircle(pos.x, pos.y, 15.0, BLACK);
    }
}
