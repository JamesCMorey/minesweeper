#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "stage.h"
#include "media.h"
#include "evloop.h"


int main() {

    srand(time(NULL));
    stage_init();
    stage_print();
    media_init();
    resources_init();

    eventloop_run();

    resources_cleanup();
    media_cleanup();
    return 0;
}
