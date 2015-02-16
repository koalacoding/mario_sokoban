#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "functions.h"

int main()
{
    int window_width = 0, window_height = 0;

    SDL_Surface *window = NULL; // window = The main window.

    SDL_Init(SDL_INIT_VIDEO);

    window_width = 408;
    window_height = 408;
    window = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    load_main_screen(window);

    load_game(window, window_height, window_width);

    SDL_Quit();
    return EXIT_SUCCESS;
}
