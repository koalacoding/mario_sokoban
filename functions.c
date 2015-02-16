#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "functions.h"

/*----------------------------------------
------------------------------------------
----------LOADING THE MAIN SCREEN---------
------------------------------------------
----------------------------------------*/


void load_main_screen(SDL_Surface* window, SDL_Rect surface_position, SDL_Event event) {
    SDL_Surface *text = NULL;
    TTF_Font *font = NULL;
    SDL_Color black_color = {0, 0, 0};
    int continue_loop = 1;

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_WM_SetCaption("Mario Sokoban", NULL);

    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    font = TTF_OpenFont("ubuntu-b.ttf", 30); // Loading the font.

    surface_position.x = 100;
    surface_position.y = 20;

    text = TTF_RenderUTF8_Blended(font, "Mario Sokoban", black_color);

    // We clean the window.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    // We blit the text to the window.
    SDL_BlitSurface(text, NULL, window, &surface_position);
    SDL_Flip(window);


    while (continue_loop)
    {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continue_loop = 0;
                break;
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();

    SDL_FreeSurface(text);
}

/*----------------------------------------
------------------------------------------
--------------LOADING THE GAME------------
------------------------------------------
----------------------------------------*/


void load_game(SDL_Surface* window, int window_height, int window_width, SDL_Rect surface_position,
                SDL_Event event) {
    SDL_Surface *squares[144] = {NULL}, *blank_square = NULL, *wall_square = NULL,
                *objective_square = NULL, *box_square = NULL, *placed_box_surface = NULL,
                *mario_surface = NULL;

    int map_data[12][12]; // This 2d array will contain the map of the game level.
    char map_filename[] = "maps/map0.map";

    // Filling the window with the white color.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    load_map(map_filename, map_data);

    // Loading the images in some SDL_Surfaces.
    blank_square = IMG_Load("sprites/blank.jpg");
    wall_square = IMG_Load("sprites/mur.jpg");
    objective_square = IMG_Load("sprites/objectif.png");
    box_square = IMG_Load("sprites/caisse.jpg");
    placed_box_surface = IMG_Load("sprites/caisse_ok.jpg");

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

    mario_surface = IMG_Load("sprites/mario_bas.gif");

    // We will put Mario in the fourth line of the window, in the 6th square.
    mario_position.x = (window_width / 12) * 5;
    mario_position.y = (window_height / 12) * 3;

    // At the beginning, Mario is in the 6th square of the fourth line.
    mario_square_position.x = 5;
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
                                    move_box(window, window_height, map_data, blank_square,
                                             box_square, placed_box_surface,
                                             mario_position, mario_square_position,
                                             &number_of_placed_boxes, 0);
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
                                    move_box(window, window_height, map_data, blank_square,
                                             box_square, placed_box_surface,
                                             mario_position, mario_square_position,
                                             &number_of_placed_boxes, 1);
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
                                    move_box(window, window_height, map_data, blank_square,
                                             box_square, placed_box_surface,
                                             mario_position, mario_square_position,
                                             &number_of_placed_boxes, 2);
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
                                    move_box(window, window_height, map_data, blank_square,
                                             box_square, placed_box_surface,
                                             mario_position, mario_square_position,
                                             &number_of_placed_boxes, 3);
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
}

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

// Function to determine if Mario can move. Mode 0 = up, 1 = down, 2 = right, 3 = left.
int can_move_mario(int map_data[][12], int mario_square_nb_x,
int mario_square_nb_y, int mode) {
	/* Up case :
			If Mario is in the first line.
	   Down case :
	   		If Mario is in the last line.
	   Right case :
	   		If Mario is in the last column.
       Left case :
            If Mario is in the first column.

	   Then Mario cannot move.*/
	if ((mode == 0 && mario_square_nb_y == 0)
		 || (mode == 1 && mario_square_nb_y == 11)
		 || (mode == 2 && mario_square_nb_x == 11)
		 || (mode == 3 && mario_square_nb_x == 0)) {
		return 0;
	}

	/* Up case :
			If Mario is in the second line and if there is a box on top of him.
	   Down case :
	   		If Mario is in the second last line and if there is a box below him.
	   Right case :
	   		If Mario is in the second last column and if there is a box at his right.
       Left case :
            If Mario is in the second column and if there is a box at his left.

	   Then he cannot move. */
	if ((mode == 0 && mario_square_nb_y == 1
		 	&& map_data[mario_square_nb_x][mario_square_nb_y - 1] == 2)
		 || (mode == 1 && mario_square_nb_y == 10
		 	&& map_data[mario_square_nb_x][mario_square_nb_y + 1] == 2)
		 || (mode == 2 && mario_square_nb_x == 10
		 	&& map_data[mario_square_nb_x + 1][mario_square_nb_y] == 2)
		 || (mode == 3 && mario_square_nb_x == 1
		 	&& map_data[mario_square_nb_x - 1][mario_square_nb_y] == 2)) {
		return 0;
	}

	/* Mario can only walk on blank squares
	   and on boxes old positions after he pushes them.*/

    switch (mode) {
        case 0: // Up case.
            if (map_data[mario_square_nb_x][mario_square_nb_y - 1] != 0
                && map_data[mario_square_nb_x][mario_square_nb_y - 1] != 2) {
                return 0;
            }
            break;
        case 1: // Down case.
            if (map_data[mario_square_nb_x][mario_square_nb_y + 1] != 0
                && map_data[mario_square_nb_x][mario_square_nb_y + 1] != 2) {
                return 0;
            }
            break;
        case 2: // Down case.
            if (map_data[mario_square_nb_x + 1][mario_square_nb_y] != 0
                && map_data[mario_square_nb_x + 1][mario_square_nb_y] != 2) {
                return 0;
            }
            break;
        case 3: // Down case.
            if (map_data[mario_square_nb_x - 1][mario_square_nb_y] != 0
                && map_data[mario_square_nb_x - 1][mario_square_nb_y] != 2) {
                return 0;
            }
            break;
    }

	/* If there is a box up / below / at the right / left of Mario,
	   and if there is a box or a wall up / below / at the right / left of this box,
	   then Mario cannot move. */
	if ((mode == 0 && map_data[mario_square_nb_x][mario_square_nb_y - 1] == 2
			&& (map_data[mario_square_nb_x][mario_square_nb_y - 2] == 1
				|| map_data[mario_square_nb_x][mario_square_nb_y - 2] == 2))
		|| (mode == 1 && map_data[mario_square_nb_x][mario_square_nb_y + 1] == 2
				&& (map_data[mario_square_nb_x][mario_square_nb_y + 2] == 1
					|| map_data[mario_square_nb_x][mario_square_nb_y + 2] == 2))
		|| (mode == 2 && map_data[mario_square_nb_x + 1][mario_square_nb_y] == 2
				&& (map_data[mario_square_nb_x + 2][mario_square_nb_y] == 1
					|| map_data[mario_square_nb_x + 2][mario_square_nb_y] == 2))
		|| (mode == 3 && map_data[mario_square_nb_x - 1][mario_square_nb_y] == 2
				&& (map_data[mario_square_nb_x - 2][mario_square_nb_y] == 1
					|| map_data[mario_square_nb_x - 2][mario_square_nb_y] == 2))) {
		return 0;
	}

	return 1;
}

/*------------------------------
------------MOVE MARIO----------
------------------------------*/

// Function to move Mario.
void move_mario(int window_width, SDL_Surface* blank_square, SDL_Surface* mario_surface,
                     SDL_Rect square_position, SDL_Rect* mario_position,
                     MarioSquarePosition* mario_square_nb,
                     SDL_Surface* main_window, int mode) {
    // We blit a blank square to Mario's old position.
    SDL_BlitSurface(blank_square, NULL, main_window, mario_position);

    // We refresh the new x,y coordinates and square number of Mario.
    switch (mode) {
        case 0: // Up.
            (*mario_position).y -= (window_width / 12);
            (*mario_square_nb).y -= 1;
            break;
        case 1: // Down.
            (*mario_position).y += (window_width / 12);
            (*mario_square_nb).y += 1;
            break;
        case 2: // Right.
            (*mario_position).x += (window_width / 12);
            (*mario_square_nb).x += 1;
            break;
        case 3: // Left.
            (*mario_position).x -= (window_width / 12);
            (*mario_square_nb).x -= 1;
            break;
    }

    // We blit Mario at its new position.
    SDL_BlitSurface(mario_surface, NULL, main_window, mario_position);

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

// Function to determine if we can push a box. Mode 0 = up, 1 = down, 2 = right, 3 = left.
int can_move_box(int map_data[][12], int mode, MarioSquarePosition mario_square_nb) {
    /* If Mario tries to push a box... :

            ...up, if he's before the second line...
            ...down, if he's after the second last line...
            ...right, if he's after the second last column...
            ...left, if he's before the second column...

        Then he cannot push the box.*/
    if ((mode == 0 && mario_square_nb.y < 2) // Up case.
        || (mode == 1 && mario_square_nb.y > 9) // Down case.
        || (mode == 2 && mario_square_nb.x > 9) // Right case
        || (mode == 3 && mario_square_nb.x < 2)) { // Left case.
        return 0;
    }

    switch (mode) { // Checking the square behind the box.
        case 0: // Up case.
            if (map_data[mario_square_nb.x][mario_square_nb.y - 2] == 1
                || map_data[mario_square_nb.x][mario_square_nb.y - 2] == 2) {
                return 0;
            }
            break;
        case 1: // Down case.
            if (map_data[mario_square_nb.x][mario_square_nb.y + 2] == 1
                || map_data[mario_square_nb.x][mario_square_nb.y + 2] == 2) {
                return 0;
            }
            break;
        case 2: // Right case.
            if (map_data[mario_square_nb.x + 2][mario_square_nb.y] == 1
                || map_data[mario_square_nb.x + 2][mario_square_nb.y] == 2) {
                return 0;
            }
            break;
        case 3: // Left case.
            if (map_data[mario_square_nb.x - 2][mario_square_nb.y] == 1
                || map_data[mario_square_nb.x - 2][mario_square_nb.y] == 2) {
                return 0;
            }
            break;
        }

    return 1;
}

/*------------------------------
------------MOVE BOXES----------
------------------------------*/

// Function to push a box.
void move_box(SDL_Surface* window, int window_height, int map_data[][12],
              SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Surface* placed_box_surface,
              SDL_Rect mario_position, MarioSquarePosition mario_square_nb, int* number_of_placed_boxes,
              int mode) {
    int is_box_placed = 0;

    /* As a box is pushed, we update the map data and we put the xy coordinates of Mario
       to the box's old position to blit a blank square there. */
    switch (mode) {
        case 0: // Up case.
            // If the box is going to be pushed on an objective.
            if (map_data[mario_square_nb.x][mario_square_nb.y - 2] == 3) {
                is_box_placed = 1;
            }

            map_data[mario_square_nb.x][mario_square_nb.y - 1] = 0;
            map_data[mario_square_nb.x][mario_square_nb.y - 2] = 2;
            mario_position.y -= (window_height / 12);

            break;
        case 1: // Down case.
            if (map_data[mario_square_nb.x][mario_square_nb.y + 2] == 3) {
                is_box_placed = 1;
            }

            map_data[mario_square_nb.x][mario_square_nb.y + 1] = 0;
            map_data[mario_square_nb.x][mario_square_nb.y + 2] = 2;
            mario_position.y += (window_height / 12);
            break;
        case 2: // Right case.
            if (map_data[mario_square_nb.x + 2][mario_square_nb.y] == 3) {
                is_box_placed = 1;
            }

            map_data[mario_square_nb.x + 1][mario_square_nb.y] = 0;
            map_data[mario_square_nb.x + 2][mario_square_nb.y] = 2;
            mario_position.x += (window_height / 12);
            break;
        case 3: // Left case.
            if (map_data[mario_square_nb.x - 2][mario_square_nb.y] == 3) {
                is_box_placed = 1;
            }

            map_data[mario_square_nb.x - 1][mario_square_nb.y] = 0;
            map_data[mario_square_nb.x - 2][mario_square_nb.y] = 2;
            mario_position.x -= (window_height / 12);
            break;
    }

    /* We move the mario_position coordinates to the square on top of Mario
    (at the old box position) and we blit a blank square.*/
    SDL_BlitSurface(blank_square, NULL, window, &mario_position);

    /* We update the xy coordinates of Mario's position
       to the new box position to blit a box there. */
    switch (mode) {
        case 0: // Up case.
            mario_position.y -= (window_height / 12);
            mario_square_nb.y -= 2;
            break;
        case 1: // Down case.
            mario_position.y += (window_height / 12);
            mario_square_nb.y += 2;
            break;
        case 2: // Right case.
            mario_position.x += (window_height / 12);
            mario_square_nb.x += 2;
            break;
        case 3: // Left case.
            mario_position.x -= (window_height / 12);
            mario_square_nb.x -= 2;
            break;
    }

    if (is_box_placed) {
        transform_box(map_data, window, placed_box_surface, mario_position, mario_square_nb,
                      number_of_placed_boxes);
        return;
    }

    // We blit the box to its new position.
    SDL_BlitSurface(box_square, NULL, window, &mario_position);
}

/*------------------------------
------------PLACED BOX---------
------------------------------*/

// Function that transforms a box to a placed box.
void transform_box(int map_data[][12], SDL_Surface* main_window,
                   SDL_Surface* placed_box_surface, SDL_Rect mario_position,
                   MarioSquarePosition mario_square_nb, int* number_of_placed_boxes) {
    *number_of_placed_boxes += 1;

    // We update the map data to indicate that the box is now a placed box.
    map_data[mario_square_nb.x][mario_square_nb.y] = 4;

    // We blit the placed box image to the box position.
    SDL_BlitSurface(placed_box_surface, NULL, main_window, &mario_position);
}
