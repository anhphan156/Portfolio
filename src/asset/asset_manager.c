#include "asset/asset_manager.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

void am_load_assets(AssetManager *am) {
    FILE *stream = fopen("gamedata/textures.data", "r");
    if (stream) {

        char   *line_buffer = 0;
        size_t  len         = 0;
        ssize_t read;

        read = getline(&line_buffer, &len, stream);
        if (read == -1) {
            printf("cant read file config/textures.data");
            exit(1);
        }

        am->count  = atoi(line_buffer);
        am->assets = malloc(sizeof(Texture2D) * am->count);

        int i = 0;
        while ((read = getline(&line_buffer, &len, stream)) != -1) {
            line_buffer[read - 1] = 0;
            am->assets[i]         = LoadTexture(line_buffer);
            i += 1;
        }

        fclose(stream);
    }
}

void am_destroy_assets(AssetManager *am) {
    UnloadTexture(am->assets[0]);
    for (int i = 0; i < am->count; i += 1) {
        UnloadTexture(am->assets[i]);
    }
    free(am->assets);
}
