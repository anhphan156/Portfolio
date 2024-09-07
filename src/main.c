#ifndef TEST
#include "application.h"

int main() {
    app_init();
    app_run();
    app_destroy();

    return 0;
}

#else
#include "testcase/testmain.h"
int main() {
    testmain();
    return 0;
}
#endif
