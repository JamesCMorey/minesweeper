#pragma once

#include <stdint.h>
#include <stdbool.h>

#define TILE16_MAGIC_NUM 0xCB
#define TILE8_MAGIC_NUM 0b11

typedef uint8_t Tile;

/* Set info in Tile */
static inline void tile_toggle_flagged(Tile *t) { *t ^= (0x1 << 7); }
static inline void tile_open(Tile *t) { *t |= (0x1 << 5); }
static inline void tile_set_num(Tile *t, uint8_t num) {
    *t &= 0xFFF0; /* Zero lower bits */
    *t |= num & 0x0F; /* set Tile lower bits to num */
}

/* Get info from Tile */
static inline uint8_t tile_num(Tile t) { return t & 0x0F; }
static inline bool tile_opened(Tile t) { return (t >> 5) & 0x1; }
static inline bool tile_flagged(Tile t) { return (t >> 7) & 0x1; }

typedef enum {
    STEP_CLEAR = 0,
    STEP_MINE,
    STEP_BLOCKED
} StepType;

//typedef enum {
//    MOVE_UP,
//    MOVE_DOWN,
//    MOVE_RIGHT,
//    MOVE_LEFT
//} MoveSelected;

//typedef struct  {
//    int row, col;
//} Coords;

typedef struct {
    int rows, cols;
    float density;
    int tiles_opened;
    Tile **board;
//    Coords selected;
} Board;

bool board_flagged(const Board *b, int row, int col);
bool board_opened(const Board *b, int row, int col);
uint8_t board_num(const Board *b, int row, int col);
int board_total_opened(const Board *b);
int board_total_mines(const Board *b);

Board *board_new(int rows, int cols, float density);

void board_toggle_flag(Board *b, int row, int col);
StepType board_open(Board *b, int row, int col);
Tile **board_read();
//void move_selection(MoveSelected move);
//Coords coords_selected();

void board_print(Board *b);
