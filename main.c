#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "functions.h"

int main()
{
    SDL_Surface *window = NULL; // window = The main window.

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_SetVideoMode(408, 408, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    load_main_screen(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}
