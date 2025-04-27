#pragma once

#include "media.h"
#include "board.h"

static inline int board_width_x(const Board *b) { return ((TILE_SIZE + 5) * b->cols); }
static inline int board_height_y(const Board *b) { return ((TILE_SIZE + 5) * b->rows);}
static inline int board_start_x(const Board *b) { return (WIN_WIDTH/2 - board_width_x(b)/2);}
static inline int board_start_y(const Board *b) { return (WIN_HEIGHT/2 - board_height_y(b)/2);}

