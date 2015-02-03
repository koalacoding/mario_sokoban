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

    SDL_Surface *window = NULL, *squares[144] = {NULL}; // window = The main window.
    SDL_Rect character_position; // Will contain x and y positions of the character.
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    window_width = 640;
    window_height = 480;
    window = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban", NULL); // Title of the window.

    //character = SDL_LoadBMP("sprites/sdl_icone.bmp"); // Sprite of the character.

    // Filling the window with the white color.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    char map_filename[] = "maps/map0.map";
    load_map(map_filename, map_data);

    // Loading the right images in every square.
    for (x = 0; x < 12; x++) {
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

            i++;
        }
    }

    //character_position.x = 0;
    //character_position.y = 0;

    //SDL_FillRect(squares[0][0], NULL, SDL_MapRGB(squares[0][0]->format, 0, 0, 0));

    //SDL_BlitSurface(squares[0][0], NULL, window, &character_position);

    SDL_Flip(window); // It is mandatory to show the window.

    while (continue_program) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continue_program = 0;
                break;
        }
    }

    // For loop to free all the surfaces of the array "squares".
    /*int i = 0;
    for (i = 0; i < 144; i++) {
       SDL_FreeSurface(squares[i]);
    }*/

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
            fscanf(myfile, "%d", &map_data[y][x]);
        }
    }

    fclose(myfile);
}
