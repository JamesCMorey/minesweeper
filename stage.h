#pragma once

#include <signal.h>
#include <stdint.h>

#define STAGE_WIDTH 10
#define STAGE_HEIGHT 10
#define DENSITY .12

typedef enum StepType {
    STEP_CLEAR = 0,
    STEP_MINE,
} StepType;

typedef enum {
    TILE_CLOSED = 0,
    TILE_OPENED ,
    TILE_FLAGGED,
} TileStatus;

typedef enum {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_RIGHT,
    MOVE_LEFT
} MoveSelected;

typedef struct  {
    int row, col;
} Coords;

typedef struct {
    uint8_t nums[STAGE_HEIGHT][STAGE_WIDTH];
    TileStatus state[STAGE_HEIGHT][STAGE_WIDTH];
    Coords selected;
} Stage;

extern Stage stage;

void stage_init();

void place_flag(int row, int col);
int stage_step(int row, int col);
const Stage *stage_read();
void move_selection(MoveSelected move);
Coords coords_selected();

void stage_print();
