//#include "SDL_events.h"
#include "media.h"
#include "board.h"
#include "layout.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

void eventloop_run(Board *b) {
    bool mouse_down = false;
    bool running = true;
    while (running) {
        SDL_Event ev;
        SDL_PollEvent(&ev);
        switch (ev.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:;
                if (!mouse_down)
                    mouse_down = true;
                else
                    break;

                int row = (ev.button.y - board_start_y(b)) / (TILE_SIZE + 5);
                int col = (ev.button.x - board_start_x(b)) / (TILE_SIZE + 5);
                if (ev.button.button == SDL_BUTTON_LEFT && row >= 0 && col >= 0) {
                    board_open(b, row, col);
                }
                else if (ev.button.button == SDL_BUTTON_RIGHT) {
                    board_toggle_flag(b, row, col);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                mouse_down = false;
        }

        draw_frame(b);
    }
}
