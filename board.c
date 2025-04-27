#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

bool board_flagged(const Board *b, int row, int col) { return tile_flagged(b->board[row][col]); }
bool board_opened(const Board *b, int row, int col) { return tile_opened(b->board[row][col]); }
uint8_t board_num(const Board *b, int row, int col) { return tile_num(b->board[row][col]); }
int board_total_opened(const Board *b) { return b->tiles_opened; }
int board_total_mines(const Board *b) { return b->rows * b->cols * b->density; }

void board_print(Board *b) {
    for (int r = 0; r < b->rows; r++) {
        for (int c = 0; c < b->cols; c++) {
            printf("%d ", tile_num(b->board[r][c]));
        }
        puts("");
    }
}

static inline void increment_surround(Board *b, int row, int col) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i + row < 0 || b->rows <= i + row) /* Invalid row */
                continue;
            if (j + col < 0 || b->cols <= j + col) /* Invalid col */
                continue;
            if (!(i || j)) /* Center tile */
                continue;

            if (tile_num(b->board[row + i][col + j]) != 9)
                tile_set_num(&b->board[row + i][col + j],
                             tile_num(b->board[row + i][col + j] + 1));
        }
    }
}

Board *board_new(int rows, int cols, float density) {
    Board *b = malloc(sizeof(*b));
    b->rows = rows;
    b->cols = cols;
    b->density = density;

    uint8_t *tmp = calloc(sizeof(uint8_t), rows*cols);
    b->board = malloc(sizeof(uint8_t*) * rows);

    for (int i = 0; i < rows; i++) {
        b->board[i] = tmp + (i * cols);
    }

    int mines = 0;
    int row, col;
    while (mines < density * rows*cols) {
        row = rand() % rows;
        col = rand() % cols;

        if (tile_num(b->board[row][col]) != 9) {
            tile_set_num(&b->board[row][col], 9);
            increment_surround(b, row, col);
            mines++;
        }
    }

    return b;
}


/* USER ACTIONS */
void board_toggle_flag(Board *b, int row, int col) {
    if(!tile_opened(b->board[row][col]))
        tile_toggle_flagged(&b->board[row][col]);
}

static void board_auto_open(Board *b, int row, int col);

StepType board_open(Board *b, int row, int col) {
    if (tile_flagged(b->board[row][col])|| tile_opened(b->board[row][col]))
        return STEP_BLOCKED;

    if (b->board[row][col] == 9) {
        return STEP_MINE;
    }

    tile_open(&b->board[row][col]);
    ++b->tiles_opened;
    if (tile_num(b->board[row][col]) == 0) {
        board_auto_open(b, row, col);
    }

    return STEP_CLEAR;
}

static void board_auto_open(Board *b, int row, int col) {
    tile_open(&b->board[row][col]);

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (b->cols <= i + row || i + row < 0) /* Invalid row */
                continue;
            if (b->rows <= j + col || j + col < 0) /* Invalid col */
                continue;
            if (!(i || j)) /* Center tile */
                continue;

            if (tile_num(b->board[row + i][col + j]) == 0
                && !tile_opened(b->board[row + i][col + j])) {
                ++b->tiles_opened;
                board_auto_open(b, row + i, col +j);
            }
            else if (tile_num(b->board[row + i][col + j]) != 9
                     && tile_num(b->board[row + i][col + j]) != 0
                     && !tile_opened(b->board[row + i][col + j])) {
                board_open(b, row + i, col + j);
            }
        }
    }
}
