#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "board.h"
//#include "media.h"
//#include "evloop.h"

int main() {

    srand(time(NULL));
    Board *b = board_new(10, 10, .20);
    board_print(b);
    //media_init();
    //resources_init();

    //eventloop_run();

    //resources_cleanup();
    //media_cleanup();
    return 0;
}
