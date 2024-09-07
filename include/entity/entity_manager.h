#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "ds/vec.h"
#include "entity/entity.h"

typedef struct {
    Vec entities;
    Vec pending;

} EntityManager;

void    em_init(EntityManager *em);
Entity *em_create_entity(EntityManager *em);
void    em_update(EntityManager *em);
void    em_destroy(EntityManager *em);

#endif
