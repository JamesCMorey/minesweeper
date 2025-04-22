#include "media.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include "stage.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>

#define WIN_TITLE "Minesweeper Clone"
#define WIN_WIDTH 1024
#define WIN_HEIGHT 720

static struct {
    SDL_Window *win;
    SDL_Renderer *r;
    struct {
        int r, g, b, a;
    } bg;
} media;

int media_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "ERROR: SDL_Init failed on line %d with error %s", 
                __LINE__, SDL_GetError());
        return 1;
    }

    media.win = SDL_CreateWindow(WIN_TITLE, SDL_WINDOWPOS_CENTERED, 
                                 SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, 0);
    if (!media.win) {
        fprintf(stderr, "ERROR: SDL_CreateWindow failed on line %d with error %s", 
                __LINE__, SDL_GetError());
        return 1;
    }
        
    media.r = SDL_CreateRenderer(media.win, -1, 0);
    if (!media.r) {
        fprintf(stderr, "ERROR: SDL_CreateRenderer failed on line %d with error %s", 
                __LINE__, SDL_GetError());
        return 1;
    }
    
    return 0;
}

void media_cleanup() {
    SDL_DestroyRenderer(media.r);
    SDL_DestroyWindow(media.win);
    SDL_Quit();
}

#define TILE_SIZE 50
#define OFFSET(idx) (TILE_SIZE + 5) * (idx)
void render_stage(const Stage *stage) {
    for (int row = 0; row < STAGE_HEIGHT; ++row) {
        for (int col = 0; col < STAGE_WIDTH; ++col) {
            if (stage->selected.row == row && stage->selected.col == col) {
                SDL_SetRenderDrawColor(media.r, 128, 0, 0, 255);
                SDL_Rect rect = {OFFSET(col) - 2, OFFSET(row) - 2,
                                 TILE_SIZE + 4, TILE_SIZE + 4};
                SDL_RenderFillRect(media.r, &rect);
            }
            switch(stage->state[row][col]) {
                case TILE_OPENED:
                    SDL_SetRenderDrawColor(media.r, 128, 128, 128, 255);
                    break;
                case TILE_FLAGGED:
                    SDL_SetRenderDrawColor(media.r, 72, 0, 0, 255);
                    break;
                case TILE_CLOSED:
                    SDL_SetRenderDrawColor(media.r, 52, 52, 52, 255);
                    break;
                default:
                    fprintf(stderr, "Tile has invalid status\n");
                    break;
            }

            SDL_Rect rect = {OFFSET(col), OFFSET(row), TILE_SIZE, TILE_SIZE};
            SDL_RenderFillRect(media.r, &rect);
        }
    }
}

void draw_frame(const Stage *stage) {
    SDL_SetRenderDrawColor(media.r, media.bg.r, media.bg.g, media.bg.b, media.bg.a);
    SDL_RenderClear(media.r);

    render_stage(stage);

    SDL_RenderPresent(media.r);
}

void renderer_color(int r, int g, int b, int a) {
    media.bg.r = r;
    media.bg.g = g;
    media.bg.b = b;
    media.bg.a = a;
}
