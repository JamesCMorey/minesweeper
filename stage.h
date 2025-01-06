#pragma once

#define STAGE_WIDTH 10
#define STAGE_HEIGHT 10
#define DENSITY .2

void st_init();

/******************************************************************************/
/*                                User Actions                                */
/******************************************************************************/

void st_place_flag(int x, int y);

/* You died */
void st_step(int x, int y);
void st_print();
