#include "stage.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

static void mine_gen();
static int count_surround(int row, int col);
static void num_gen();

static int stage[STAGE_HEIGHT][STAGE_WIDTH] = { 0 };

void st_init() {
    srand(time(NULL));

    mine_gen();
    st_print();
    num_gen();
    puts("");
}

static void num_gen() {
    for (int row = 0; row < STAGE_HEIGHT; row++) {
        for (int col = 0; col < STAGE_WIDTH; col++) {
            if (stage[row][col] == 9)
                continue;
            stage[row][col] = count_surround(row, col);
        }
    }
}

static int count_surround(int row, int col) {
    int mine_count = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i >= STAGE_WIDTH || j >= STAGE_HEIGHT || (!i && !j))
                continue;

            if (stage[row + i][col + j] == 9)
                mine_count++;
        }
    }

    return mine_count;
}

static void mine_gen() {
    int x, y;
    int mines = 0;
    while (mines < DENSITY * STAGE_WIDTH * STAGE_HEIGHT) {
        x = rand() % STAGE_WIDTH;
        y = rand() % STAGE_HEIGHT;

        if (stage[y][x] != 9) {
            stage[y][x] = 9;
            mines++;
        }
    }
}

void st_print() {
    for (int r = 0; r < STAGE_HEIGHT; r++) {
        for (int c = 0; c < STAGE_WIDTH; c++) {
            printf("%d ", stage[r][c]);
        }
        puts("");
    }

}

/******************************************************************************/
/*                                User Actions                                */
/******************************************************************************/

void st_place_flag(int x, int y) {

}

/* You died */
void st_step(int x, int y) {

}
