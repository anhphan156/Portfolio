#ifndef CANIMATION_H
#define CANIMATION_H

typedef struct {
    char     animation_enabled;
    long int animation_live_frame;
    int      current_animation_frame;
    int      animation_duration;
} CAnimation;

#endif
