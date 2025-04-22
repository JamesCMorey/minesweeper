#include "SDL_events.h"
#include "media.h"
#include "stage.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

void eventloop_run() {
    const Stage *stage = stage_read();
    bool running = true;
    while (running) {
        SDL_Event ev;
        SDL_PollEvent(&ev);
        switch (ev.type) {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:;
                int row = ev.button.y / (TILE_SIZE + 5);
                int col = ev.button.x / (TILE_SIZE + 5);
                if (ev.button.button == SDL_BUTTON_LEFT) {
                    stage_step(row, col);
                }
                break;

            case SDL_KEYDOWN: /* Keyboard */
                switch(ev.key.keysym.scancode) {
                    case SDL_SCANCODE_S:
                        move_selection(MOVE_DOWN);
                        break;
                    case SDL_SCANCODE_D:
                        move_selection(MOVE_RIGHT);
                        break;
                    case SDL_SCANCODE_A:
                        move_selection(MOVE_LEFT);
                        break;
                    case SDL_SCANCODE_W:
                        move_selection(MOVE_UP);
                        break;
                    case SDL_SCANCODE_SPACE:
                        if (stage_step(stage->selected.row, stage->selected.col) == STEP_MINE)
                            ; /* add handler */
                        break;
                    case SDL_SCANCODE_F:
                        place_flag(stage->selected.row, stage->selected.col);
                        break;
                    default:
                        break;
                }
        }

        draw_frame(stage_read());
    }
}
