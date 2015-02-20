#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "game_window/game_window.h"
#include "main_window/main_window.h"

int main()
{
    SDL_Surface *window = NULL; // window = The main window.

    SDL_Init(SDL_INIT_VIDEO);

    load_main_window(window, 0);

    SDL_Quit();

    return EXIT_SUCCESS;
}
