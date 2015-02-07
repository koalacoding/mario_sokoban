void load_map (char* filename, int map_data[][12]);

// Function to determine if Mario can move up.
int can_mario_move_up(int map_data[][12], int mario_square_x, int mario_square_y);

// Function to determine if Mario can move down.
int can_mario_move_down(int map_data[][12], int mario_square_x, int mario_square_y);
