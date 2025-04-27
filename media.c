#include "media.h"
//#include "SDL_render.h"
//#include "SDL_surface.h"
//#include "SDL_video.h"
#include "board.h"
#include "layout.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define WIN_TITLE "Minesweeper Clone"
#define TEXT_SIZE 30

#define CREATE_COLOR(r, g, b, a) (SDL_Color){r, g, b, a}
#define RED         CREATE_COLOR(255, 0, 0, 255)
#define GREEN       CREATE_COLOR(0, 255, 0, 255)
#define BLUE        CREATE_COLOR(0, 0, 255, 255)
#define YELLOW      CREATE_COLOR(255, 255, 0, 255)
#define CYAN        CREATE_COLOR(0, 255, 255, 255)
#define MAGENTA     CREATE_COLOR(255, 0, 255, 255)
#define ORANGE      CREATE_COLOR(255, 165, 0, 255)
#define PURPLE      CREATE_COLOR(128, 0, 128, 255)

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
        SDL_Surface *surface;

        #define SURFACE_COLOR(color) surface = TTF_RenderText_Blended(media.font, num_str, color)
        switch(i) {
            case 1:
                SURFACE_COLOR(RED);
                break;
            case 2:
                SURFACE_COLOR(GREEN);
                break;
            case 3:
                SURFACE_COLOR(BLUE);
                break;
            case 4:
                SURFACE_COLOR(PURPLE);
                break;
            case 5:
                SURFACE_COLOR(CYAN);
                break;
            case 6:
                SURFACE_COLOR(ORANGE);
                break;
            case 7:
                SURFACE_COLOR(MAGENTA);
                break;
            case 8:
                SURFACE_COLOR(YELLOW);
                break;
            default:
                SURFACE_COLOR(RED);
                break;
        }

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
void render_stage(const Board *b) {
    for (int row = 0; row < b->rows; ++row) {
        for (int col = 0; col < b->cols; ++col) {
            /* Outline selected tile */
            //if (stage->selected.row == row && b->selected.col == col) {
            //    SDL_SetRenderDrawColor(media.r, 128, 0, 0, 255);
            //    SDL_Rect rect = {STAGE_START_X + OFFSET(col) - 2, STAGE_START_Y + OFFSET(row) - 2,
            //                     TILE_SIZE + 4, TILE_SIZE + 4};
            //    SDL_RenderFillRect(media.r, &rect);
            //}

            /* Set tile color */
            if (board_opened(b, row, col)) SDL_SetRenderDrawColor(media.r, 128, 128, 128, 255);
            else if (board_flagged(b, row, col)) SDL_SetRenderDrawColor(media.r, 72, 0, 0, 255);
            else SDL_SetRenderDrawColor(media.r, 52, 52, 52, 255);

            /* Draw tile background*/
            SDL_Rect rect = {
                board_start_x(b) + OFFSET(col),
                board_start_y(b) + OFFSET(row),
                TILE_SIZE,
                TILE_SIZE
            };
            SDL_RenderFillRect(media.r, &rect);

            /* Draw tile number over background */
            int num = board_num(b, row, col);
            if (num > 0 && board_opened(b, row, col))
                SDL_RenderCopy(media.r, num_textures[num - 1], NULL, &rect);
        }
    }
}

void render_game_state(const Board *b) {
    if (board_total_opened(b) >= b->rows * b->cols - board_total_mines(b)) {
        SDL_SetRenderDrawColor(media.r, 100, 100, 100, 255);
        SDL_Rect bg_rect = {
            board_start_x(b) + board_width_x(b)/2 - 230/2 - 3, 
            board_start_y(b) + board_height_y(b)/2 - 100/2 - 3, 
            230,
            100
        };
        SDL_RenderFillRect(media.r, &bg_rect);

        /* 'You win' text TODO: Unhardcode all of this */
        SDL_SetRenderDrawColor(media.r, 255, 100, 100, 255);
        SDL_Rect rect = {
            board_start_x(b) + board_width_x(b)/2 - 200/2 - 3,
            board_start_y(b) + board_height_y(b)/2 - 100/2 + 3,
            200,
            100
        };
        SDL_RenderCopy(media.r, win_text, NULL, &rect);
    }
}

void draw_frame(const Board *b) {
    SDL_SetRenderDrawColor(media.r, media.bg.r, media.bg.g, media.bg.b, media.bg.a);
    SDL_RenderClear(media.r);

    render_stage(b);
    render_game_state(b);

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
