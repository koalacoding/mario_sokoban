#include "../structures.h"

/*----------------------------------------
------------------------------------------
--------------LOADING THE GAME------------
------------------------------------------
----------------------------------------*/


void load_game(int map_number);


/*----------------------------------------
------------------------------------------
--------LOAD AND BLIT WINDOW DESIGN-------
------------------------------------------
----------------------------------------*/


void load_and_blit_window_design(SDL_Surface* window,
                                    SDL_Surface* *pointer_on_black_bar_vertical,
                                    SDL_Surface* *pointer_on_exit_button);


/*----------------------------------------
------------------------------------------
-----------------THE MAP------------------
------------------------------------------
----------------------------------------*/


/*------------------------------
----------LOADING THE MAP-------
------------------------------*/

int load_map (char* filename, int map_data[][12]);

/*------------------------------
----------DRAWING THE MAP-------
------------------------------*/

void draw_map(int map_data[][12], SDL_Surface* window, SDL_Surface* squares[144],
                SDL_Surface* wall_surface, SDL_Surface* box_surface,
                SDL_Surface* objective_surface, SDL_Surface* mario_surface, int* number_of_boxes,
                SDL_Rect* mario_xy, MarioSquarePosition* mario_square_nb, int game_window_width,
                int game_window_height);

/*----------------------------------------
------------------------------------------
-----------LOADING GAME SPRITES-----------
------------------------------------------
----------------------------------------*/


void load_game_sprites(SDL_Surface* *pointer_on_blank_sprite,
                        SDL_Surface* *pointer_on_mario_sprite,
                        SDL_Surface* *pointer_on_wall_sprite,
                        SDL_Surface* *pointer_on_objective_sprite,
                        SDL_Surface* *pointer_on_box_sprite,
                        SDL_Surface* *pointer_on_placed_box_sprite);


/*----------------------------------------
------------------------------------------
--------------MARIO MOVEMENTS-------------
------------------------------------------
----------------------------------------*/


/*------------------------------
---------CAN OBJECT MOVE ?------
------------------------------*/

int can_move_object(int mode, int object_square_nb_x, int object_square_nb_y, int is_object_box,
                        int next_square_type);

int can_move_mario(int map_data[][12], int mario_square_nb_x,
                    int mario_square_nb_y, int mode);

/*------------------------------
------------MOVE MARIO----------
------------------------------*/

void move_(int mode, int map_data[][12], MarioSquarePosition* mario_square_nb,
            SDL_Surface* blank_square, SDL_Surface* window, SDL_Rect* mario_xy,
            int game_window_width, int game_window_height,
            int mario_square_nb_x, int mario_square_nb_y,
            SDL_Surface* mario_surface, SDL_Surface* box_surface, SDL_Surface* placed_box_surface,
            int* number_of_placed_boxes);

void move_object(SDL_Surface* blank_square, SDL_Surface* window, SDL_Rect* object_position,
                    int is_object_mario, int mode, int game_window_width, int game_window_height, MarioSquarePosition* mario_square_nb,
                    int object_square_nb_x, int object_square_nb_y, int map_data[][12],
                    SDL_Surface* mario_surface, SDL_Surface* box_surface, SDL_Surface* placed_box_surface,
                    int* number_of_placed_boxes);

// Function to move Mario.
void move_mario(int game_window_width, SDL_Surface* blank_square, SDL_Surface* mario_surface,
                     SDL_Rect* mario_position, MarioSquarePosition* mario_square_nb,
                     SDL_Surface* main_window, int mode);


/*----------------------------------------
------------------------------------------
-------------------BOXES------------------
------------------------------------------
----------------------------------------*/


/*------------------------------
---------CAN MOVE BOXES ?-------
------------------------------*/

// Function to determine if we can push a box. Mode 0 = up, 1 = down, 2 = right, 3 = left.
int can_move_box(int map_data[][12], int mode, MarioSquarePosition mario_square_nb);

/*------------------------------
------------MOVE BOXES----------
------------------------------*/

// Function to move a box.
void move_box(SDL_Surface* window, int window_height, int map_data[][12],
              SDL_Surface* blank_square, SDL_Surface* box_square, SDL_Surface* placed_box_surface,
              SDL_Rect mario_position, MarioSquarePosition mario_square_nb, int* number_of_placed_boxes,
              int mode);

/*------------------------------
------------PLACED BOX---------
------------------------------*/

// Function that transforms a box to a placed box.
void transform_box(int map_data[][12], SDL_Surface* main_window,
                   SDL_Surface* placed_box_surface, SDL_Rect mario_position,
                   MarioSquarePosition mario_square_nb, int* number_of_placed_boxes);

int have_all_boxes_been_placed(int number_of_boxes, int number_of_placed_boxes);


/*----------------------------------------
------------------------------------------
----------CLOSE THE GAME WINDOW-----------
------------------------------------------
----------------------------------------*/


void close_game_window(SDL_Surface* black_bar_vertical, SDL_Surface* exit_button,
                        SDL_Surface* blank_square, SDL_Surface* wall_square,
                        SDL_Surface* objective_square, SDL_Surface* box_square,
                        SDL_Surface* placed_box_surface, SDL_Surface* mario_surface);
