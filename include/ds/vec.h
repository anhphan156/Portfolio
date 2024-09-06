#ifndef VEC_H
#define VEC_H

#include "ds/linkedlist.h"

typedef struct {
    Node        *head;
    unsigned int length;
} Vec;

void  vec_push(Vec *, void *);
void *vec_pop(Vec *);
void  vec_remove(Vec *, int (*predicate)(void *));
void  vec_flush(Vec *);

#endif
