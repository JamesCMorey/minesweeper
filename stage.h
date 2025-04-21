#pragma once

#define STAGE_WIDTH 10
#define STAGE_HEIGHT 10
#define DENSITY .12

typedef enum StepType {
    STEP_CLEAR = 0,
    STEP_MINE,
} StepType;

void stage_print();

void stage_init();

void place_flag(int row, int col);
int stage_step(int row, int col);
