#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "prototypes.h"

/* Information about which square is Mario in.
typedef struct mario_location mario_location;
struct mario_location
{
    int x;
    int y;
};*/

int main()
{
    int continue_program = 1;

    int window_width = 0, window_height = 0;

    int map_data[12][12]; // This 2d array will contain the map of the game level.

    int x = 0, y = 0, i = 0;

    SDL_Surface *window = NULL, *squares[144] = {NULL}, *blank_square = NULL, *box_square = NULL, *mario = NULL; // window = The main window.
    SDL_Rect surface_position, mario_position; // Will contain x and y positions to place the surfaces containing images.
    SDL_Event event;

    MarioSquarePosition mario_square_position;

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

    blank_square = IMG_Load("sprites/blank.jpg");

    mario = IMG_Load("sprites/mario_bas.gif");

    // We will put Mario on the top of the window, in the 6th square.
    mario_position.x = (window_width / 12) * 5;
    mario_position.y = 0;

    // At the beginning, Mario is in the 6th square of the first line.
    mario_square_position.x = 5;
    mario_square_position.y = 0;

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
                        if (can_mario_move_up(map_data, mario_square_position.x,
                        mario_square_position.y) == 1) {
                            // If there is a box on top of Mario.
                            if (map_data[mario_square_position.x][mario_square_position.y - 1]
                            == 2) {
                                // If we can move a box up.
                                if (can_move_box_up(map_data, mario_square_position.x,
                                mario_square_position.y) == 1) {
                                    // Then we move it up.
                                    move_box_up(window_height, map_data,
                                    mario_square_position, mario_position, window);
                                }
                            }
                            // We move Mario up.
                            move_mario_up(window_height, surface_position, &mario_position,
                            &mario_square_position, window, mario, blank_square);
                        }

                        break;

                    case SDLK_DOWN:
                        if (can_mario_move_down(map_data, mario_square_position.x, mario_square_position.y)
                        == 1) {
                            surface_position.x = mario_position.x;
                            surface_position.y = mario_position.y;

                            mario_position.y += (window_height / 12);
                            SDL_BlitSurface(mario, NULL, window, &mario_position);

                            mario_square_position.y += 1;

                            SDL_BlitSurface(blank_square, NULL, window, &surface_position);

                            SDL_Flip(window);
                        }
                        break;

                    case SDLK_RIGHT:
                        if (can_mario_move_right(map_data, mario_square_position.x, mario_square_position.y)
                        == 1) {
                            surface_position.x = mario_position.x;
                            surface_position.y = mario_position.y;

                            mario_position.x += (window_height / 12);
                            SDL_BlitSurface(mario, NULL, window, &mario_position);

                            mario_square_position.x += 1;

                            SDL_BlitSurface(blank_square, NULL, window, &surface_position);

                            SDL_Flip(window);
                        }

                        break;

                    case SDLK_LEFT:
                        if (can_mario_move_left(map_data, mario_square_position.x,
                        mario_square_position.y)
                        == 1) {
                            surface_position.x = mario_position.x;
                            surface_position.y = mario_position.y;

                            mario_position.x -= (window_height / 12);
                            SDL_BlitSurface(mario, NULL, window, &mario_position);

                            mario_square_position.x -= 1;

                            SDL_BlitSurface(blank_square, NULL, window, &surface_position);

                            SDL_Flip(window);
                        }

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


/*---------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------FUNCTIONS---------------------------------------
-----------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------
---------------------------------------------------------------------------------------*/



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


/*----------------------------------------
------------------------------------------
--------------MARIO MOVEMENTS-------------
------------------------------------------
----------------------------------------*/


/*------------------------------
---------CAN MARIO MOVE ?-------
------------------------------*/

// Function to determine if Mario can move up.
int can_mario_move_up(int map_data[][12], int mario_square_x, int mario_square_y) {
    int can_move_up = 1; // 0 = false, 1 = true.

    if (mario_square_y == 0) { // If Mario is in the first line of the map, then he can't move up.
        can_move_up = 0;
    }

    else { // If Mario is not in the first line.
        if (mario_square_y == 1) { // If Mario is in the second line...
            // ...then, if there is a box in the upper square, he won't be able to move up.
            if (map_data[mario_square_x][mario_square_y - 1] == 2) {
                can_move_up = 0;
            }
        }
        /* If the square on top of Mario is a wall (1),
        or if it is an objective, then Mario cannot move up. */
        if (map_data[mario_square_x][mario_square_y - 1] == 1
        || map_data[mario_square_x][mario_square_y - 1] == 3) {
            can_move_up = 0;
        }

        /* If there is a box on top of Mario,
        and there is a wall on top of the box, then Mario won't be able to move up. */
        if (map_data[mario_square_x][mario_square_y - 1] == 2
        && map_data[mario_square_x][mario_square_y - 2] == 1) {
            can_move_up = 0;
        }
    }

    return can_move_up;
}

// Function to determine if Mario can move down.
int can_mario_move_down(int map_data[][12], int mario_square_position_x,
int mario_square_position_y) {
    int can_move_down = 1; // 0 = false, 1 = true.

    // If Mario is in the last line of the map, then he can't move down.
    if (mario_square_position_y == 11) {
        can_move_down = 0;
    }

    else { // If Mario is not in the last line.
        if (mario_square_position_y == 10) { // If Mario is in the second last line...
            // ...then, if there is a box below him, he won't be able to move down.
            if (map_data[mario_square_position_x][mario_square_position_y + 1] == 2) {
                can_move_down = 0;
            }
        }
        /* If the square below Mario is a wall (1),
        or if it is an objective (3), then Mario cannot move down. */
        if (map_data[mario_square_position_x][mario_square_position_y + 1] == 1
        || map_data[mario_square_position_x][mario_square_position_y + 1] == 3) {
            can_move_down = 0;
        }

        if (mario_square_position_y < 10) { // If Mario is over the second last line.
            /* If there is a box below Mario,
            and there is a wall on below the box, then Mario won't be able to move down. */
            if (map_data[mario_square_position_x][mario_square_position_y + 1] == 2
            && map_data[mario_square_position_x][mario_square_position_y + 2] == 1) {
                can_move_down = 0;
            }
        }
    }

    return can_move_down;
}

// Function to determine if Mario can move to the right.
int can_mario_move_right(int map_data[][12], int mario_square_x, int mario_square_y) {
    int can_move_right = 1; // 0 = false, 1 = true.

    // If Mario is in the last column of the map, then he can't move to the right.
    if (mario_square_x == 11) {
        can_move_right = 0;
    }

    else { // If Mario is not in the last column.
        /* If the square to the right of Mario is a wall (1),
        or if it is an objective, then Mario cannot move to the right. */
        if (map_data[mario_square_x + 1][mario_square_y] == 1
        || map_data[mario_square_x + 1][mario_square_y] == 3) {
            can_move_right = 0;
        }
    }

    return can_move_right;
}

// Function to determine if Mario can move to the left.
int can_mario_move_left(int map_data[][12], int mario_square_x, int mario_square_y) {
    int can_move_left = 1; // 0 = false, 1 = true.

    // If Mario is in the first column of the map, then he can't move to the left.
    if (mario_square_x == 0) {
        can_move_left = 0;
    }

    else { // If Mario is not in the first column.
        /* If the square to the left of Mario is a wall (1),
        or if it is an objective, then Mario cannot move to the left. */
        if (map_data[mario_square_x - 1][mario_square_y] == 1
        || map_data[mario_square_x - 1][mario_square_y] == 3) {
            can_move_left = 0;
        }
    }

    return can_move_left;
}

/*------------------------------
------------MOVE MARIO----------
------------------------------*/

// Function to move Mario up.
void move_mario_up(int window_height, SDL_Rect square_position, SDL_Rect* mario_position,
MarioSquarePosition* mario_location, SDL_Surface* main_window, SDL_Surface* mario_surface,
SDL_Surface* blank_surface) {
    square_position.x = (*mario_position).x;
    square_position.y = (*mario_position).y;

    // We blit Mario at its new position.
    (*mario_position).y -= (window_height / 12);
    SDL_BlitSurface(mario_surface, NULL, main_window, mario_position);

    (*mario_location).y -= 1;

    // We blit a blank square to Mario's old position.
    SDL_BlitSurface(blank_surface, NULL, main_window, &square_position);

    // We refresh the window to show the changes.
    SDL_Flip(main_window);
}


/*----------------------------------------
------------------------------------------
-------------------BOXES------------------
------------------------------------------
----------------------------------------*/


/*------------------------------
---------CAN MOVE BOXES ?-------
------------------------------*/

// Function to determine if Mario can move a box up.
int can_move_box_up(int map_data[][12], int mario_square_x, int mario_square_y) {
    int can_move_box_up = 1; // 0 = false, 1 = true.

    if (mario_square_y <= 1) { // If Mario tries to push a box that is in the first line.
        can_move_box_up = 0;
    }

    else { // If Mario is below the second line.
        // If Mario tries to push a box in a square where there is a wall or a box.
        if (map_data[mario_square_x][mario_square_y - 2] == 1
        || map_data[mario_square_x][mario_square_y - 2] == 2) {
            can_move_box_up = 0;
        }
    }

    return can_move_box_up;
}

/*------------------------------
------------MOVE BOXES----------
------------------------------*/

// Function to move a box up.
void move_box_up(int window_height, int map_data[][12],
MarioSquarePosition mario_square_position, SDL_Rect mario_position, SDL_Surface* window) {
    SDL_Surface *blank_square = NULL, *box_square = NULL;
    SDL_Rect surface_position;

    // We update the map data after the box is pushed up.
    map_data[mario_square_position.x][mario_square_position.y - 1] = 0;
    map_data[mario_square_position.x][mario_square_position.y - 2] = 2;

    // We put a blank square at the old box's position.
    surface_position.x = mario_position.x;
    // Position of the square on top of Mario.
    surface_position.y = mario_position.y - (window_height / 12);
    blank_square = IMG_Load("sprites/blank.jpg");
    SDL_BlitSurface(blank_square, NULL, window, &surface_position);

    // We load the box image in the box_square surface.
    box_square = IMG_Load("sprites/caisse.jpg");
    surface_position.y = mario_position.y - ((window_height / 12) * 2);
    SDL_BlitSurface(box_square, NULL, window, &surface_position);
}
