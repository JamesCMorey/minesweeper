#include "stage.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

Stage stage = { 0 };

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
    while (mines < MINE_NUM) {
        row = rand() % STAGE_HEIGHT;
        col = rand() % STAGE_WIDTH;

        if (stage.nums[row][col] != 9) {
            stage.nums[row][col] = 9;
            increment_surround(row, col);
            mines++;
        }
    }
    stage.selected.row = 1;
    stage.selected.col = 1;
}

void stage_clear() {
    memset(&stage, 0, sizeof(stage));
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
    if(stage.state[row][col] == TILE_CLOSED)
        stage.state[row][col] = TILE_FLAGGED;
    else if (stage.state[row][col] == TILE_FLAGGED){
        stage.state[row][col] = TILE_CLOSED;
    }
}

static void stage_auto_step(int row, int col) {
    stage.state[row][col] = TILE_OPENED;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (STAGE_WIDTH <= i + row || i + row < 0) /* Invalid row */
                continue;
            if (STAGE_HEIGHT <= j + col || j + col < 0) /* Invalid col */
                continue;
            if (!(i || j)) /* Center tile */
                continue;

            if (stage.nums[row + i][col + j] == 0
                && stage.state[row + i][col + j] == TILE_CLOSED) {
                ++stage.tiles_opened;
                stage_auto_step(row + i, col +j);
            }
            else if (stage.nums[row + i][col + j] != 9 && stage.nums[row + i][col + j] != 0 && stage.state[row + i][col + j] != TILE_OPENED) {
                stage_step(row + i, col + j);
            }
        }
    }
}

StepType stage_step(int row, int col) {
    if (stage.state[row][col] == TILE_FLAGGED || stage.state[row][col] == TILE_OPENED)
        return STEP_BLOCKED;

    if (stage.nums[row][col] == 9)
        return STEP_MINE;

    stage.state[row][col] = TILE_OPENED;
    ++stage.tiles_opened;
    if (stage.nums[row][col] == 0) {
        stage_auto_step(row, col);
    }

    return STEP_CLEAR;
}


const Stage *stage_read() {
    return &stage;
}

void move_selection(MoveSelected move) {
    switch(move) {
        case MOVE_UP:
            if (stage.selected.row > 0)
                --stage.selected.row;
            break;
        case MOVE_DOWN:
            if (stage.selected.row < STAGE_HEIGHT - 1)
                ++stage.selected.row;
            break;
        case MOVE_LEFT:
            if (stage.selected.col > 0)
                --stage.selected.col;
            break;
        case MOVE_RIGHT:
            if (stage.selected.col < STAGE_WIDTH - 1)
                ++stage.selected.col;
            break;
    }
}

Coords coords_selected() {
    return (Coords) { stage.selected.row, stage.selected.col };
}
