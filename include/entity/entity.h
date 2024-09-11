#ifndef ENTITY_H
#define ENTITY_H

#include "component/canimation.h"
#include "component/cbbox.h"
#include "component/cinput.h"
#include "component/cshape.h"
#include "component/cstate.h"
#include "component/ctexture.h"
#include "component/ctransform.h"

typedef struct {
    CTransform transform;
    CTexture   texture;
    CShape     shape;
    CBBox      bbox;
    CInput     input;
    CState     state;
    CAnimation animation;
    int        is_alive;
} Entity;

#endif
