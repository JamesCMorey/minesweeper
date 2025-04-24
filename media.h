#pragma once

#include "stage.h"

#define TILE_SIZE 50
#define WIN_WIDTH 1024
#define WIN_HEIGHT 720
#define STAGE_START_X (WIN_WIDTH/2 - ((TILE_SIZE + 5) * STAGE_WIDTH)/2)
#define STAGE_START_Y (WIN_HEIGHT/2 - ((TILE_SIZE + 5) * STAGE_HEIGHT)/2)

int media_init();
void media_cleanup();
void resources_init();
void resources_cleanup();

void draw_frame(const Stage *stage);
void renderer_color(int r, int g, int b, int a);

void show_win();
