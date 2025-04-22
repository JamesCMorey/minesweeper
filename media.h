#pragma once

#include "stage.h"

int media_init();
void media_cleanup();

void draw_frame(const Stage *stage);
void renderer_color(int r, int g, int b, int a);
