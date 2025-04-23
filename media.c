#include "media.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_video.h"
#include "stage.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define WIN_TITLE "Minesweeper Clone"
#define WIN_WIDTH 1024
#define WIN_HEIGHT 720
#define TEXT_SIZE 30

#define RED (SDL_Color){255, 0, 0, 255}
#define BLUE {0, 255, 0, 255}
#define GREEN {0, 0, 255, 255}

static SDL_Texture *num_textures[9];
static SDL_Texture *win_text;

static struct {
    SDL_Window *win;
    SDL_Renderer *r;
    struct {
        int r, g, b, a;
    } bg;
    TTF_Font *font;
} media;

int media_init() {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        fprintf(stderr, "ERROR: SDL_Init failed on line %d with error %s", 
                __LINE__, SDL_GetError());
        return 1;
    }

    if (TTF_Init()) {
        fprintf(stderr, "ERROR: TTF_Init failed on line %d with error %s", 
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

void resources_init() {
    media.font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE);
    if (!media.font) {
        fprintf(stderr, "Error creating Font: %s\n", TTF_GetError());
        return;
    }

    for (int i = 1; i <= 9; ++i) {
        char num_str[2];
        sprintf(num_str, "%d", i);
        SDL_Surface *surface = TTF_RenderText_Blended(media.font, num_str, RED);
        if (!surface) {
            fprintf(stderr, "Error creating Surface: %s\n", SDL_GetError());
            return;
        }

        num_textures[i - 1] = SDL_CreateTextureFromSurface(media.r, surface);
        SDL_FreeSurface(surface);
        if (!num_textures[i - 1]) {
            fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
            return;
        }
    }

    SDL_Surface *surface = TTF_RenderText_Blended(media.font, "You win", RED);
    if (!surface) {
        fprintf(stderr, "Error creating Surface: %s\n", SDL_GetError());
        return;
    }

    win_text = SDL_CreateTextureFromSurface(media.r, surface);
    SDL_FreeSurface(surface);
    if (!win_text) {
        fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
        return;
    }
}

void media_cleanup() {
    SDL_DestroyRenderer(media.r);
    SDL_DestroyWindow(media.win);
    SDL_Quit();
}

void resources_cleanup() {
    TTF_CloseFont(media.font);
    TTF_Quit();
}

#define OFFSET(idx) (TILE_SIZE + 5) * (idx)
void render_stage(const Stage *stage) {
    for (int row = 0; row < STAGE_HEIGHT; ++row) {
        for (int col = 0; col < STAGE_WIDTH; ++col) {
            /* Outline selected tile */
            if (stage->selected.row == row && stage->selected.col == col) {
                SDL_SetRenderDrawColor(media.r, 128, 0, 0, 255);
                SDL_Rect rect = {OFFSET(col) - 2, OFFSET(row) - 2,
                                 TILE_SIZE + 4, TILE_SIZE + 4};
                SDL_RenderFillRect(media.r, &rect);
            }

            /* Draw tile backgrounds */
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

            /* Draw numbers */
            int num = stage->nums[row][col];
            if (num > 0 && stage->state[row][col] == TILE_OPENED) {
                SDL_RenderCopy(media.r, num_textures[num - 1], NULL, &rect);
            }
        }
    }

    if (stage->tiles_opened >= STAGE_HEIGHT * STAGE_WIDTH - MINE_NUM) {
        SDL_Rect rect = {500, 100, 100, 100};
        SDL_RenderCopy(media.r, win_text, NULL, &rect);
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

void show_win() {
}
