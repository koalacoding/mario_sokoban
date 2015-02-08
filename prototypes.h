// Information about which square is Mario in.
typedef struct MarioSquarePosition MarioSquarePosition;
struct MarioSquarePosition
{
    int x;
    int y;
};

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
int can_mario_move_up(int map_data[][12], int mario_square_x, int mario_square_y);
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
MarioSquarePosition* mario_location, SDL_Surface* main_window, SDL_Surface* mario_surface,
SDL_Surface* blank_surface);


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

/*------------------------------
------------MOVE BOXES----------
------------------------------*/

// Function to move a box up.
void move_box_up(int window_height, int map_data[][12],
MarioSquarePosition mario_square_position, SDL_Rect mario_position, SDL_Surface* window);
