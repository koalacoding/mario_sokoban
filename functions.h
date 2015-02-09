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

// Function to determine if Mario can move up.
int can_mario_move_up(int map_data[][12], int mario_square_position_x,
                      int mario_square_position_y);

// Function to determine if Mario can move down.
int can_mario_move_down(int map_data[][12], int mario_square_position_x,
                        int mario_square_position_y);

// Function to determine if Mario can move to the right.
int can_mario_move_right(int map_data[][12], int mario_square_x, int mario_square_y);

// Function to determine if Mario can move to the left.
int can_mario_move_left(int map_data[][12], int mario_square_x, int mario_square_y);

/*------------------------------
------------MOVE MARIO----------
------------------------------*/

// Function to move Mario up.
void move_mario_up (int window_height, SDL_Rect square_position, SDL_Rect* mario_position,
MarioSquarePosition* mario_square_location, SDL_Surface* main_window);

// Function to move Mario down.
void move_mario_down(int window_height, SDL_Rect square_position, SDL_Rect* mario_position,
MarioSquarePosition* mario_square_location, SDL_Surface* main_window);

// Function to move Mario to the right.
void move_mario_right(int window_width, SDL_Rect square_position, SDL_Rect* mario_position,
MarioSquarePosition* mario_square_location, SDL_Surface* main_window);

// Function to move Mario to the left.
void move_mario_left(int window_width, SDL_Surface* blank_square, SDL_Surface* mario_surface,
                     SDL_Rect square_position, SDL_Rect* mario_position,
                     MarioSquarePosition* mario_square_location, SDL_Surface* main_window);


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
