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

	/* If there is a wall (1) or an objective (3)... :

	   Up case :
			...in top of Mario.
	   Down case :
	   		...below Mario.
	   Right case :
	   		...at the right of Mario.
       Left case :
            ...at the left of Mario.

	   Then he cannot move. */
	if ((mode == 0 && ((map_data[mario_square_nb_x][mario_square_nb_y - 1] % 2) != 0))
        || (mode == 1 && ((map_data[mario_square_nb_x][mario_square_nb_y + 1] % 2) != 0))
        || (mode == 2 &&((map_data[mario_square_nb_x + 1][mario_square_nb_y] % 2) != 0))
        || (mode == 3 &&((map_data[mario_square_nb_x - 1][mario_square_nb_y] % 2) != 0))) {
		return 0;
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

// Function to determine if Mario can move a box to the right.
int can_move_box_right(int map_data[][12], int mario_square_x, int mario_square_y) {
    int can_move_box_right = 1; // 0 = false, 1 = true.

    /* If Mario is in the last column or the second last column,
    then he cannot push a box to the right. */
    if (mario_square_x >= 10) {
        can_move_box_right = 0;
    }

    else { // If Mario is before the second last column.
        // If Mario tries to push a box in a square where there is a wall (1) or a box (2).
        if (map_data[mario_square_x + 2][mario_square_y] == 1
        || map_data[mario_square_x + 2][mario_square_y] == 2) {
            can_move_box_right = 0;
        }
    }

    return can_move_box_right;
}

// Function to determine if Mario can move a box to the left.
int can_move_box_left(int map_data[][12], int mario_square_x, int mario_square_y) {
    int can_move_box_left = 1; // 0 = false, 1 = true.

    /* If Mario is in the first column or the second column,
    then he cannot push a box to the left. */
    if (mario_square_x <= 1) {
        can_move_box_left = 0;
    }

    else { // If Mario is after the second column.
        // If Mario tries to push a box in a square where there is a wall (1) or a box (2).
        if (map_data[mario_square_x - 2][mario_square_y] == 1
        || map_data[mario_square_x - 2][mario_square_y] == 2) {
            can_move_box_left = 0;
        }
    }

    return can_move_box_left;
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
    (at the old box position) and we blit a blank square.*/
    mario_position.y -= (window_height / 12);
    SDL_BlitSurface(blank_square, NULL, window, &mario_position);

    /* We move the mario_position coordinates 1 square more on top of Mario
    (at the new box position). */
    mario_position.y -= (window_height / 12);
    // We blit the box to its new position.
    SDL_BlitSurface(box_square, NULL, window, &mario_position);
}

// Function to move a box down.
void move_box_down(SDL_Surface* window, int window_height, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position) {
    // We update the map data as the box is pushed down.
    map_data[mario_position.x / (window_height / 12)]
            [mario_position.y / (window_height / 12) + 1] = 0;
    map_data[mario_position.x / (window_height / 12)]
            [mario_position.y / (window_height / 12) + 2] = 2;

    /* We move the mario_position coordinates to the square below Mario
    (at the old box position) and we blit a blank square. */
    mario_position.y += (window_height / 12);
    SDL_BlitSurface(blank_square, NULL, window, &mario_position);

    /* We move the mario_position coordinates 1 square more below Mario
    (at the new box position). */
    mario_position.y += (window_height / 12);
    // We blit the box to its new position.
    SDL_BlitSurface(box_square, NULL, window, &mario_position);
}

// Function to move a box to the right.
void move_box_right(SDL_Surface* window, int window_width, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position) {
    // We update the map data as the box is pushed to the right.
    map_data[mario_position.x / (window_width / 12) + 1]
            [mario_position.y / (window_width / 12)] = 0;
    map_data[mario_position.x / (window_width / 12) + 2]
            [mario_position.y / (window_width / 12)] = 2;

    /* We move the mario_position coordinates to the square at the right of Mario
    (at the old box position) and we blit a blank square. */
    mario_position.x += (window_width / 12);
    SDL_BlitSurface(blank_square, NULL, window, &mario_position);

    /* We move the mario_position coordinates 1 square more to the right of Mario
    (at the new box position). */
    mario_position.x += (window_width / 12);
    // We blit the box to its new position.
    SDL_BlitSurface(box_square, NULL, window, &mario_position);
}

// Function to move a box to the left.
void move_box_left(SDL_Surface* window, int window_width, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position) {
    // We update the map data as the box is pushed to the left.
    map_data[mario_position.x / (window_width / 12) - 1]
            [mario_position.y / (window_width / 12)] = 0;
    map_data[mario_position.x / (window_width / 12) - 2]
            [mario_position.y / (window_width / 12)] = 2;

    /* We move the mario_position coordinates to the square at the left of Mario
    (at the old box position) and we blit a blank square. */
    mario_position.x -= (window_width / 12);
    SDL_BlitSurface(blank_square, NULL, window, &mario_position);

    /* We move the mario_position coordinates 1 square more to the left of Mario
    (at the new box position). */
    mario_position.x -= (window_width / 12);
    // We blit the box to its new position.
    SDL_BlitSurface(box_square, NULL, window, &mario_position);
}
