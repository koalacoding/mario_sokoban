#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "prototypes.h"

int main()
{
    int continue_program = 1;

    int window_width = 0, window_height = 0;

    int map_data[12][12]; // This 2d array will contain the map of the game level.

    int x = 0, y = 0, i = 0;

    SDL_Surface *window = NULL, *squares[144] = {NULL}, *blank_square = NULL, *mario = NULL; // window = The main window.
    SDL_Rect surface_position, mario_position; // Will contain x and y positions to place the surfaces containing images.
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    window_width = 408;
    window_height = 408;
    window = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban", NULL); // Title of the window.

    //character = SDL_LoadBMP("sprites/sdl_icone.bmp"); // Sprite of the character.

    // Filling the window with the white color.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    char map_filename[] = "maps/map0.map";
    load_map(map_filename, map_data);

    surface_position.x = 0;
    surface_position.y = 0;

    // Loading the right images in every square.
    for (x = 0; x < 12; x++) {

        // We need to reset surface_position.y at 0 after each y coordinate for loop.
        surface_position.y = 0;

        for (y = 0; y < 12; y++) {

            if (map_data[x][y] == 1) {
                squares[i] = IMG_Load("sprites/mur.jpg");
            }

            else if (map_data[x][y] == 2) {
                squares[i] = IMG_Load("sprites/caisse.jpg");
            }

            else if (map_data[x][y] == 3) {
                squares[i] = IMG_Load("sprites/objectif.png");
            }

            // We make the square appear on the window.
            SDL_BlitSurface(squares[i], NULL, window, &surface_position);

            surface_position.y += (window_height / 12);

            i++; // "i" should get bigger until it reaches the total number of squares surfaces.
        }
        /* Through each loop, we add to the x coordinate
        of the surface position 1/12 of the window width. */
        surface_position.x += (window_width / 12);
    }

    mario = IMG_Load("sprites/mario_bas.gif");

    // We will put Mario on the top of the window, in the 6th square.
    mario_position.x = (window_width / 12) * 5;
    mario_position.y = 0;

    SDL_BlitSurface(mario, NULL, window, &mario_position);

    SDL_Flip(window); // It is mandatory to show the window.

    blank_square = IMG_Load("sprites/blank.jpg");

    while (continue_program) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continue_program = 0;
                break;
            case SDL_KEYDOWN: // Cases when we press a key.
                switch(event.key.keysym.sym) {
                     case SDLK_UP:
                        surface_position.x = mario_position.x;
                        surface_position.y = mario_position.y;

                        mario_position.y -= (window_height / 12);
                        SDL_BlitSurface(mario, NULL, window, &mario_position);

                        SDL_BlitSurface(blank_square, NULL, window, &surface_position);

                        SDL_Flip(window);
                        break;

                    case SDLK_DOWN:
                        surface_position.x = mario_position.x;
                        surface_position.y = mario_position.y;

                        // We blit Mario at its new position.
                        mario_position.y += (window_height / 12);
                        SDL_BlitSurface(mario, NULL, window, &mario_position);

                        // We blit a blank square to Mario's old position.
                        SDL_BlitSurface(blank_square, NULL, window, &surface_position);

                        // We refresh the window to show the changes.
                        SDL_Flip(window);
                        break;

                    case SDLK_RIGHT:
                        surface_position.x = mario_position.x;
                        surface_position.y = mario_position.y;

                        mario_position.x += (window_height / 12);
                        SDL_BlitSurface(mario, NULL, window, &mario_position);

                        SDL_BlitSurface(blank_square, NULL, window, &surface_position);

                        SDL_Flip(window);
                        break;

                    case SDLK_LEFT:
                        surface_position.x = mario_position.x;
                        surface_position.y = mario_position.y;

                        mario_position.x -= (window_height / 12);
                        SDL_BlitSurface(mario, NULL, window, &mario_position);

                        SDL_BlitSurface(blank_square, NULL, window, &surface_position);

                        SDL_Flip(window);
                        break;
                }
                break;
        }
    }

    // For loop to free all the surfaces of the array "squares".
    for (i = 0; i < 144; i++) {
       SDL_FreeSurface(squares[i]);
    }

    SDL_Quit();
    return EXIT_SUCCESS;
}


/*----------------------------------
--------------FUNCTIONS-------------
----------------------------------*/

void load_map(char* filename, int map_data[][12]) {
    FILE* myfile;

    myfile = fopen(filename, "rb");

    if (myfile == NULL) {
        printf("Cannot open %s", filename);
        exit(1);
    }

    int y = 0, x = 0;

    for (y = 0; y < 12; y++) {
        for (x = 0; x < 12; x++) {
            fscanf(myfile, "%d", &map_data[x][y]);
        }
    }

    fclose(myfile);
}
