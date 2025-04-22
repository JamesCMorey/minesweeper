#include <stdlib.h>
#include <time.h>
#include "stage.h"
#include "media.h"
#include "evloop.h"


int main() {

    srand(time(NULL));
    stage_init();
    stage_print();
    media_init();

    eventloop_run();

    media_cleanup();
    return 0;
}
