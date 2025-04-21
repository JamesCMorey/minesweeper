#include <stdlib.h>
#include <time.h>
#include "stage.h"


int main() {

    srand(time(NULL));
    stage_init();
    stage_print();

    return 0;
}
