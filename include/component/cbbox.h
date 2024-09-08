#ifndef CBBOX_H
#define CBBOX_H

#include "raylib.h"

typedef struct {
    Vector2 collision_axes;
    Vector2 prev_collision_axes;
    Vector2 overlapped_shape;
} AABBResult;

typedef struct {
    int        enabled;
    Vector2    half_box;
    AABBResult collision_result;
} CBBox;

#endif
