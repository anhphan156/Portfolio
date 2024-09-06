#include "ds/vec.h"
#include <stdio.h>
#include <stdlib.h>

void vec_push(Vec *v, void *data) {
    if (v->head == 0) {
        v->head = malloc(sizeof(Node));

        if (!v->head) {
            printf("failed to allocate memory");
            return;
        }

        v->head->data = data;
        v->head->next = 0;
        v->length     = 1;
        return;
    }

    Node *new = malloc(sizeof(Node));
    if (!new) {
        printf("failed to allocate memory");
        return;
    }
    new->data = data;
    new->next = v->head;
    v->head   = new;
    v->length += 1;
}

void *vec_pop(Vec *v) {
    Node *tmp = v->head;
    if (tmp == 0) {
        return 0;
    }

    void *data = tmp->data;
    v->head    = tmp->next;
    v->length -= 1;
    free(tmp);
    return data;
}

void vec_remove(Vec *v, int (*predicate)(void *)) {
    // remove the rest
    Node *prev = v->head;
    if (prev == 0) {
        return;
    }
    Node *cur           = prev->next;
    Node *to_be_deleted = 0;

    while (cur != 0) {
        int is_alive = predicate(cur->data);
        if (is_alive == 0) {
            to_be_deleted = cur;
            cur           = cur->next;
            prev->next    = cur;

            if (to_be_deleted->data) {
                free(to_be_deleted->data);
            }
            free(to_be_deleted);
            to_be_deleted = 0;
            v->length -= 1;
        } else {
            prev = prev->next;
            cur  = cur->next;
        }
    }

    // remove head if predicate is true;
    if (predicate(v->head->data) == 0) {
        free(vec_pop(v));
    }
}

void vec_flush(Vec *v) {
    unsigned int len = v->length;
    for (int i = 0; i < len; i += 1) {
        void *e = vec_pop(v);
        if (e) {
            free(e);
        }
    }
}
