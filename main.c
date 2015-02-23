// STANDARD HEADERS
#include <stdio.h>
#include <stdlib.h>

// EXTERNAL HEADERS
#include <SDL/SDL.h>

// CUSTOM HEADERS
#include "status.h"
#include "window.h"
#include "board.h"

static const char* map_filename = "maps/map0.map";

int main(int argc, char *argv[]) {
    Status status;
    Board* board = NULL;
    Window* window = NULL;
    SDL_Event event;
    bool exit_event_loop = false;

    board = board_create(map_filename);
    if (board == NULL) {
        goto end;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        goto end;
    }

    // do /2 for testing window resizing
    window = window_create("Mario Sokoban", 408/2, 408/2);
    if (window == NULL) {
        fprintf(stderr, "Cannot create main window\n");
        goto end;
    }

    status = window_display_map(window, board->map);
    if (status.code != MARIO_STATUS_SUCCESS) {
        return status.code;
    }

    while (exit_event_loop == false) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                exit_event_loop = true;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        board_move_mario(board, DIRECTION_UP);
                        break;
                    case SDLK_DOWN:
                        board_move_mario(board, DIRECTION_DOWN);
                        break;
                    case SDLK_LEFT:
                        board_move_mario(board, DIRECTION_LEFT);
                        break;
                    case SDLK_RIGHT:
                        board_move_mario(board, DIRECTION_RIGHT);
                        break;
                }
                // TODO: optimization (this draws all window)
                window_display_map(window, board->map);
            break;
        }
    }
end:
    if (window) {
        window_destroy(window);
    }
    SDL_Quit();

    if (board) {
        board_destroy(board);
    }
    return 0;
}
