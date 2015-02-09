#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "functions.h"

int main()
{
    int continue_program = 1;

    int window_width = 0, window_height = 0;

    int map_data[12][12]; // This 2d array will contain the map of the game level.

    int x = 0, y = 0, i = 0;

    // window = The main window.
    SDL_Surface *window = NULL, *squares[144] = {NULL}, *blank_square = NULL,
                *objective_square = NULL, *box_square = NULL, *wall_square = NULL, *mario = NULL;

    // Will contain x and y positions to place the surfaces containing images.
    SDL_Rect surface_position, mario_position;
    SDL_Event event;

    MarioSquarePosition mario_square_position;

    SDL_Init(SDL_INIT_VIDEO);

    window_width = 408;
    window_height = 408;
    window = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban", NULL); // Title of the window.

    // Filling the window with the white color.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    char map_filename[] = "maps/map0.map";
    load_map(map_filename, map_data);

    // Loading the images in some SDL_Surfaces.
    blank_square = IMG_Load("sprites/blank.jpg");
    wall_square = IMG_Load("sprites/mur.jpg");
    objective_square = IMG_Load("sprites/objectif.png");
    box_square = IMG_Load("sprites/caisse.jpg");

    surface_position.x = 0;
    surface_position.y = 0;

    // Loading the right images in every square.
    for (x = 0; x < 12; x++) {

        // We need to reset surface_position.y at 0 after each y coordinate for loop.
        surface_position.y = 0;

        for (y = 0; y < 12; y++) {

            if (map_data[x][y] == 1) {
                squares[i] = wall_square;
            }

            else if (map_data[x][y] == 2) {
                squares[i] = box_square;
            }

            else if (map_data[x][y] == 3) {
                squares[i] = objective_square;
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
    mario_position.x = (window_width / 12) * 6;
    mario_position.y = (window_height / 12) * 3;

    // At the beginning, Mario is in the 6th square of the first line.
    mario_square_position.x = 6;
    mario_square_position.y = 3;

    SDL_BlitSurface(mario, NULL, window, &mario_position);

    SDL_Flip(window); // It is mandatory to show the window.

    while (continue_program) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continue_program = 0;
                break;
            case SDL_KEYDOWN: // Cases when we press a key.
                switch(event.key.keysym.sym) {
                     case SDLK_UP:
                        // If we can move Mario up.
                        if (can_move_mario(map_data, mario_square_position.x,
                            mario_square_position.y, 0)) {
                            // If there is a box on top of Mario.
                            if (map_data[mario_square_position.x][mario_square_position.y - 1]
                            == 2) {
                                // If we can move a box up.
                                if (can_move_box(map_data, 0, mario_square_position)) {
                                    // Then we move it up.
                                    move_box_up(window, window_height, map_data, blank_square,
                                                box_square, mario_position);
                                }
                            }
                            move_mario(window_width, blank_square, mario, surface_position,
                            &mario_position, &mario_square_position, window, 0);
                        }

                        break;

                    case SDLK_DOWN:
                        // If we can move Mario down.
                        if (can_move_mario(map_data, mario_square_position.x,
                            mario_square_position.y, 1)) {
                            // If there is a box below Mario.
                            if (map_data[mario_square_position.x][mario_square_position.y + 1]
                            == 2) {
                                // If we can move a box down.
                                if (can_move_box(map_data, 1, mario_square_position)) {
                                    // Then we move it down.
                                    move_box_down(window, window_height, map_data, blank_square,
                                                box_square, mario_position);
                                }
                            }
                            move_mario(window_width, blank_square, mario, surface_position,
                            &mario_position, &mario_square_position, window, 1);
                        }

                        break;

                    case SDLK_RIGHT:
                        // If we can move Mario to the right.
                        if (can_move_mario(map_data, mario_square_position.x,
                            mario_square_position.y, 2)) {
                            // If there is a box in the right of Mario.
                            if (map_data[mario_square_position.x + 1][mario_square_position.y]
                            == 2) {
                                // If Mario can move a box to the right.
                                if (can_move_box(map_data, 2, mario_square_position)) {
                                    // Then we move it to the right.
                                    move_box_right(window, window_width, map_data, blank_square,
                                                  box_square, mario_position);
                                }
                            }
                            move_mario(window_width, blank_square, mario, surface_position,
                            &mario_position, &mario_square_position, window, 2);
                        }

                        break;

                    case SDLK_LEFT:
                        if (can_move_mario(map_data, mario_square_position.x,
                            mario_square_position.y, 3)) {
                            // If there is a box at the left of Mario.
                            if (map_data[mario_square_position.x - 1][mario_square_position.y]
                            == 2) {
                                 // If Mario can move a box to the left.
                                if (can_move_box(map_data, 3, mario_square_position)) {
                                    // Then we move it to the left.
                                    move_box_left(window, window_width, map_data, blank_square,
                                                  box_square, mario_position);
                                }
                            }
                            move_mario(window_width, blank_square, mario, surface_position,
                            &mario_position, &mario_square_position, window, 3);
                        }

                        break;
                }
                break;
        }
    }

    // Freeing the SDL_Surfaces.
    SDL_FreeSurface(blank_square);
    SDL_FreeSurface(wall_square);
    SDL_FreeSurface(objective_square);
    SDL_FreeSurface(box_square);
    SDL_FreeSurface(mario);

    SDL_Quit();
    return EXIT_SUCCESS;
}
