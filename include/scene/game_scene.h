#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "entity/manager.h"
#include "scene/scene.h"

typedef struct {
    EntityManager em;
    SceneType     scene_type;
    Camera2D      camera;
} GameScene;

void       game_scene_update();
GameScene *game_scene_init();
void       game_scene_destroy();

#endif
