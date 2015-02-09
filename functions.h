#include "structures.h"

/*----------------------------------------
------------------------------------------
--------------LOADING THE MAP-------------
------------------------------------------
----------------------------------------*/

void load_map (char* filename, int map_data[][12]);

/*----------------------------------------
------------------------------------------
--------------MARIO MOVEMENTS-------------
------------------------------------------
----------------------------------------*/


/*------------------------------
---------CAN MARIO MOVE ?-------
------------------------------*/

// Function to determine if Mario can move. Mode 0 = up, mode 1 = down, mode 2 = right, mode 3 = left.
int can_move_mario(int map_data[][12], int mario_square_nb_x,
int mario_square_nb_y, int mode);

/*------------------------------
------------MOVE MARIO----------
------------------------------*/

// Function to move Mario.
void move_mario(int window_width, SDL_Surface* blank_square, SDL_Surface* mario_surface,
                     SDL_Rect square_position, SDL_Rect* mario_position,
                     MarioSquarePosition* mario_square_nb,
                     SDL_Surface* main_window, int mode);

/*----------------------------------------
------------------------------------------
-------------------BOXES------------------
------------------------------------------
----------------------------------------*/


/*------------------------------
---------CAN MOVE BOXES ?-------
------------------------------*/

// Function to determine if Mario can move a box up.
int can_move_box_up(int map_data[][12], int mario_square_x, int mario_square_y);

// Function to determine if Mario can move a box down.
int can_move_box_down(int map_data[][12], int mario_square_x, int mario_square_y);

// Function to determine if Mario can move a box to the right.
int can_move_box_right(int map_data[][12], int mario_square_x, int mario_square_y);

// Function to determine if Mario can move a box to the left.
int can_move_box_left(int map_data[][12], int mario_square_x, int mario_square_y);

/*------------------------------
------------MOVE BOXES----------
------------------------------*/

// Function to move a box up.
void move_box_up(SDL_Surface* window, int window_height, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position);

// Function to move a box down.
void move_box_down(SDL_Surface* window, int window_height, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position);

// Function to move a box to the right.
void move_box_right(SDL_Surface* window, int window_width, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position);

// Function to move a box to the left.
void move_box_left(SDL_Surface* window, int window_width, int map_data[][12],
                 SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Rect mario_position);
