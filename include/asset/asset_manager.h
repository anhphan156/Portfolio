#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "raylib.h"

typedef struct {
    Texture2D *assets;
    int        count;
} AssetManager;

void am_load_assets(AssetManager *);
void am_destroy_assets(AssetManager *);

#endif
