#include "component/ctexture.h"
#include "constant.h"
#include "entity/entity_manager.h"
#include "level/level_parser.h"
#include <stdio.h>
#include <stdlib.h>

void level_parser(const char *level_name, EntityManager *em, int *cell_size) {
    FILE *stream = fopen(level_name, "r");
    if (!stream) {
        perror("failed to open level file");
        return;
    }

    char   *line_buffer = 0;
    size_t  len         = 0;
    ssize_t read        = 0;

    read = getline(&line_buffer, &len, stream);
    if (read == -1) {
        perror("getline");
        exit(1);
    }
    *cell_size = atoi(line_buffer);
    printf("%d\n", *cell_size);
    int y_offset = HEIGHT % *cell_size;

    int texture_id = NO_TEXTURE, x = 0, y = 0;
    while ((read = getline(&line_buffer, &len, stream)) != -1) {
        sscanf(line_buffer, "%d %d %d", &texture_id, &x, &y);

        printf("%d %d %d\n", texture_id, x, y);
        Entity *e               = em_create_entity(em);
        e->is_alive             = 1;
        e->transform.position.x = *cell_size * x;
        e->transform.position.y = HEIGHT - y * *cell_size - y_offset * 2;
        e->transform.velocity.x = 0.0;
        e->transform.velocity.y = 0.0;
        e->shape.r              = *cell_size;
        e->texture.id           = texture_id;
    }

    free(line_buffer);
    fclose(stream);
}
