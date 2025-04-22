#pragma once

#include "stage.h"

#define TILE_SIZE 50

int media_init();
void media_cleanup();
void resources_init();
void resources_cleanup();

void draw_frame(const Stage *stage);
void renderer_color(int r, int g, int b, int a);
