#ifndef ENTITY_H
#define ENTITY_H

#include "component/ctransform.h"

typedef struct {
    CTransform transform;
    int        is_alive;
} Entity;

#endif
