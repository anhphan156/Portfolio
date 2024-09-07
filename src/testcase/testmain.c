#include "testcase/testmain.h"
#include "entity/entity_manager.h"
#include "level/level_parser.h"
#include <stdio.h>

void level_parser_test();

void testmain() {
    level_parser_test();
}

void level_parser_test() {
    int           cell_size = 0;
    EntityManager em;
    em_init(&em);
    level_parser("gamedata/level.data", &em, &cell_size);
    printf("%d", em.pending.length);
    em_destroy(&em);
}
