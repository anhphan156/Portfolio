#include "physics/aabb_detection.h"
#include <math.h>

int AABB_detection(Vector2 a_pos, Vector2 a_half_box, Vector2 b_pos, Vector2 b_half_box, Vector2 *axes, Vector2 *area) {
    float dx = fabs(a_pos.x - b_pos.x);
    float dy = fabs(a_pos.y - b_pos.y);

    float ox = a_half_box.x + b_half_box.x;
    float oy = a_half_box.y + b_half_box.y;

    axes->x = ox > dx ? 1.0 : 0.0;
    axes->y = oy > dy ? 1.0 : 0.0;
    area->x = ox - dx;
    area->y = oy - dy;

    return axes->x * axes->y;
}
