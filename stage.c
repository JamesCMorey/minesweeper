#include "stage.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

typedef enum TileStatus {
    TILE_CLOSED = 0,
    TILE_OPENED,
    TILE_FLAGGED
} TileStatus;

typedef struct Stage {
    uint8_t nums[STAGE_HEIGHT][STAGE_WIDTH];
    TileStatus state[STAGE_HEIGHT][STAGE_WIDTH];

} Stage;

static Stage stage;

static inline void increment_surround(int row, int col) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (STAGE_WIDTH <= i + row || i + row < 0) /* Invalid row */
                continue;
            if (STAGE_HEIGHT <= j + col || j + col < 0) /* Invalid col */
                continue;
            if (!(i || j)) /* Center tile */
                continue;

            if (stage.nums[row + i][col + j] != 9)
                ++stage.nums[row + i][col  + j];
        }
    }
}

void stage_init() {
    int row, col;
    int mines = 0;
    while (mines < DENSITY * STAGE_WIDTH * STAGE_HEIGHT) {
        row = rand() % STAGE_HEIGHT;
        col = rand() % STAGE_WIDTH;

        if (stage.nums[row][col] != 9) {
            stage.nums[row][col] = 9;
            increment_surround(row, col);
            mines++;
        }
    }
}

void stage_print() {
    for (int r = 0; r < STAGE_HEIGHT; r++) {
        for (int c = 0; c < STAGE_WIDTH; c++) {
            printf("%d ", stage.nums[r][c]);
        }
        puts("");
    }
}

/* USER ACTIONS */
void place_flag(int row, int col) {
    if(stage.state[row][col] != TILE_OPENED)
        stage.state[row][col] = TILE_FLAGGED;
}

int stage_step(int row, int col) {
    if (stage.nums[row][col] == TILE_FLAGGED)
        return STEP_CLEAR;

    if (stage.nums[row][col] == 9)
        return STEP_MINE;

    stage.state[row][col] = TILE_OPENED;
    return STEP_CLEAR;
}
