#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "functions.h"

/*----------------------------------------
------------------------------------------
--------------LOADING THE MAP-------------
------------------------------------------
----------------------------------------*/


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
MarioSquarePosition* mario_square_location, SDL_Surface* main_window) {
    SDL_Surface *blank_square = NULL, *mario_surface = NULL;

    square_position.x = (*mario_position).x;
    square_position.y = (*mario_position).y;

    // We refresh the new y coordinate of Mario's position (-1/12 of the window's height);
    (*mario_position).y -= (window_height / 12);

    // We blit Mario at its new position.
    mario_surface = IMG_Load("sprites/mario_bas.gif");
    SDL_BlitSurface(mario_surface, NULL, main_window, mario_position);

    (*mario_square_location).y -= 1;

    // We blit a blank square to Mario's old position.
    blank_square = IMG_Load("sprites/blank.jpg");
    SDL_BlitSurface(blank_square, NULL, main_window, &square_position);

    // We refresh the window to show the changes.
    SDL_Flip(main_window);
}

// Function to move Mario down.
void move_mario_down(int window_height, SDL_Rect square_position, SDL_Rect* mario_position,
MarioSquarePosition* mario_square_location, SDL_Surface* main_window) {
    SDL_Surface *blank_square = NULL, *mario_surface = NULL;

    square_position.x = (*mario_position).x;
    square_position.y = (*mario_position).y;

    // We refresh the new y coordinate of Mario's position (+1/12 of the window's height);
    (*mario_position).y += (window_height / 12);

    // We blit Mario at its new position.
    mario_surface = IMG_Load("sprites/mario_bas.gif");
    SDL_BlitSurface(mario_surface, NULL, main_window, mario_position);

    // We refresh the new square coordinates of Mario.
    (*mario_square_location).y += 1;

    // We blit a blank square to Mario's old position.
    blank_square = IMG_Load("sprites/blank.jpg");
    SDL_BlitSurface(blank_square, NULL, main_window, &square_position);

    // We refresh the window to show the changes.
    SDL_Flip(main_window);
}

// Function to move Mario to the right.
void move_mario_right(int window_width, SDL_Rect square_position, SDL_Rect* mario_position,
MarioSquarePosition* mario_square_location, SDL_Surface* main_window) {
    SDL_Surface *blank_square = NULL, *mario_surface = NULL;

    square_position.x = (*mario_position).x;
    square_position.y = (*mario_position).y;

    // We refresh the new x coordinate of Mario's position (+1/12 of the window's width);
    (*mario_position).x += (window_width / 12);

    // We blit Mario at its new position.
    mario_surface = IMG_Load("sprites/mario_bas.gif");
    SDL_BlitSurface(mario_surface, NULL, main_window, mario_position);

    // We refresh the new square coordinates of Mario.
    (*mario_square_location).x += 1;

    // We blit a blank square to Mario's old position.
    blank_square = IMG_Load("sprites/blank.jpg");
    SDL_BlitSurface(blank_square, NULL, main_window, &square_position);

    // We refresh the window to show the changes.
    SDL_Flip(main_window);
}

// Function to move Mario to the left.
void move_mario_left(int window_width, SDL_Surface* blank_square, SDL_Surface* mario_surface,
                     SDL_Rect square_position, SDL_Rect* mario_position,
                     MarioSquarePosition* mario_square_location, SDL_Surface* main_window) {
    square_position.x = (*mario_position).x;
    square_position.y = (*mario_position).y;

    // We refresh the new x coordinate of Mario's position (-1/12 of the window's width);
    (*mario_position).x -= (window_width / 12);

    // We blit Mario at its new position.
    SDL_BlitSurface(mario_surface, NULL, main_window, mario_position);

    // We refresh the new square coordinates of Mario.
    (*mario_square_location).x -= 1;

    // We blit a blank square to Mario's old position.
    SDL_BlitSurface(blank_square, NULL, main_window, &square_position);

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

// Function to determine if Mario can move a box down.
int can_move_box_down(int map_data[][12], int mario_square_x, int mario_square_y) {
    int can_move_box_down = 1; // 0 = false, 1 = true.

    // If Mario is in the last line or the second last line, then he cannot push a box down.
    if (mario_square_y >= 10) {
        can_move_box_down = 0;
    }

    else { // If Mario is over the second last line.
        // If Mario tries to push a box in a square where there is a wall (1) or a box (2).
        if (map_data[mario_square_x][mario_square_y + 2] == 1
        || map_data[mario_square_x][mario_square_y + 2] == 2) {
            can_move_box_down = 0;
        }
    }

    return can_move_box_down;
}

/*------------------------------
------------MOVE BOXES----------
------------------------------*/

// Function to move a box up.
void move_box_up(SDL_Surface* window, int window_height, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position) {
    // We update the map data as the box is pushed up.
    map_data[mario_position.x / (window_height / 12)]
            [mario_position.y / (window_height / 12) - 1] = 0;
    map_data[mario_position.x / (window_height / 12)]
            [mario_position.y / (window_height / 12) - 2] = 2;

    /* We move the mario_position coordinates to the square on top of Mario
    (at the old box position). */
    mario_position.y -= (window_height / 12);
    // We put a blank square at the box's old position.
    SDL_BlitSurface(blank_square, NULL, window, &mario_position);

    /* We move the mario_position coordinates 2 squares on top of Mario
    (at the new box position). */
    mario_position.y -= (window_height / 12);
    // We blit the box to its new position.
    SDL_BlitSurface(box_square, NULL, window, &mario_position);
}
