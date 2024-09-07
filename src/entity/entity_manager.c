#include "entity/entity_manager.h"
#include "ds/vec.h"
#include "entity/entity.h"
#include <stdlib.h>

Entity *em_create(EntityManager *em) {
    Entity *e = malloc(sizeof(Entity));
    vec_push(&em->pending, e);

    return e;
}

int is_entity_alive(void *e) {
    return ((Entity *)e)->is_alive;
}

void em_update(EntityManager *em) {
    unsigned int len = em->pending.length;
    for (int i = 0; i < len; i += 1) {
        Entity *e = vec_pop(&em->pending);
        vec_push(&em->entities, e);
    }

    vec_remove(&em->entities, is_entity_alive);
}

void em_destroy(EntityManager *em) {
    vec_flush(&em->entities);
    vec_flush(&em->pending);
}
