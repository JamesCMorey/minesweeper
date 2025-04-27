#pragma once

#include "board.h"

#define TILE_SIZE 50
#define WIN_WIDTH 1024
#define WIN_HEIGHT 720

int media_init();
void media_cleanup();
void resources_init();
void resources_cleanup();

void draw_frame(const Board *b);
void renderer_color(int r, int g, int b, int a);
