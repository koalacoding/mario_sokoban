#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "map_editor.h"

void load_map_editor() {
    int window_height, window_width;

    SDL_Surface *window, *blank_square_black_border = NULL;

    SDL_Rect surface_position;

    SDL_Event event;

    int x = 0, y = 0;

    int continue_loop = 1;

    window = SDL_SetVideoMode(500, 408, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    window_width = 408;
    window_height = 408;

    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    blank_square_black_border = IMG_Load("sprites/blank_black_border.jpg");

    surface_position.x = 0; // Initialization.

    for (x = 0; x < 12; x++) { // Filling the window with black border blank squares.
        // We need to reset surface_position.y to 0 to start at the beginning of the new line.
        surface_position.y = 0;

        for (y = 0; y < 12; y++) {
            SDL_BlitSurface(blank_square_black_border, NULL, window, &surface_position);

            surface_position.y += (window_height / 12); // Going to the next square on the y axis.
        }
        // Through each loop, we add to the x coordinate 1/12 of the window width.
        surface_position.x += (window_width / 12);
    }

    SDL_Flip(window);

    while (continue_loop)
    {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                SDL_FreeSurface(blank_square_black_border);

                continue_loop = 0;
                break;

        }
    }
}
