#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

int main()
{
    int continue_program = 1;

    SDL_Surface *window = NULL, *character = NULL; // window = The main window.
    SDL_Rect character_position; // Will contain x and y positions of the character.
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban", NULL); // Title of the window.

    character = SDL_LoadBMP("sprites/sdl_icone.bmp"); // Sprite of the character.

    // Filling the window with the white color.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    character_position.x = 0;
    character_position.y = 0;

    SDL_BlitSurface(character, NULL, window, &character_position);

    SDL_Flip(window);

    while (continue_program) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continue_program = 0;
                break;
        }
    }

    SDL_FreeSurface(character);

    SDL_Quit();
    return EXIT_SUCCESS;
}
