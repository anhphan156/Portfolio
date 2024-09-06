#include "application.h"
#include "ds/vec.h"
#include "entity/manager.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    app_init();
    app_run();
    app_destroy();

    /*EntityManager em;*/
    /*em.pending.head    = 0;*/
    /*em.pending.length  = 0;*/
    /*em.entities.head   = 0;*/
    /*em.entities.length = 0;*/
    /**/
    /*for (int i = 0; i < 20; i += 1) {*/
    /*    Entity *e   = em_create(&em);*/
    /*    e->is_alive = 0;*/
    /*}*/
    /*printf("entities len: %d\n", em.entities.length);*/
    /*printf("pending len: %d\n", em.pending.length);*/
    /**/
    /*em_update(&em);*/
    /**/
    /*printf("entities len: %d\n", em.entities.length);*/
    /*printf("pending len: %d\n", em.pending.length);*/

    return 0;
}
