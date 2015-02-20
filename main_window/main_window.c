#include <SDL/SDL.h>
#include "main_window.h"
#include "../tools/write_text_on_window/write_text_on_window.h"
#include "../game_window/game_window.h"
#include "../select_map_window/select_map_window.h"
#include "../map_editor/map_editor.h"

void load_main_window(SDL_Surface* window, int selected_map_nb) {
    int continue_loop = 1;

    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_SetVideoMode(408, 408, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    SDL_WM_SetCaption("Mario Sokoban", NULL);

    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    write_text_on_window(window, 100, 20, 30, 0, 0, 0, "Mario Sokoban");
    write_text_on_window(window, 150, 150, 30, 0, 0, 0, "1. Play");
    write_text_on_window(window, 105, 200, 30, 0, 0, 0, "2. Select map");
    write_text_on_window(window, 105, 250, 30, 0, 0, 0, "3. Map editor");

    while (continue_loop)
    {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continue_loop = 0;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_KP1:
                        load_game(selected_map_nb);
                        continue_loop = 0;
                        break;
                    case SDLK_KP2:
                        load_select_map_window(window);
                        continue_loop = 0;
                        break;
                    case SDLK_KP3:
                        load_map_editor();
                        continue_loop = 0;
                        break;
                }
        }
    }

    SDL_Quit();
}
