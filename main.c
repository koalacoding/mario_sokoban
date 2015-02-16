// standard librairies
#include <stdio.h>
#include <stdlib.h>

// third party includes
#include <SDL/SDL.h>

// custom includes
#include "debug.h"
#include "window.h"
#include "board.h"

static const char* map_filename = "maps/map0.map";

int main(int argc, char *argv[]) {
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

    window = window_create("Mario Sokoban", 408, 408);
    if (window == NULL) {
        fprintf(stderr, "Cannot create main window\n");
        goto end;
    }

    while (exit_event_loop == false) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                exit_event_loop = true;
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
