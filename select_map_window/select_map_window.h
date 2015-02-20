/*----------------------------------------
------------------------------------------
---------------MINI SPRITES---------------
------------------------------------------
----------------------------------------*/


void load_mini_sprites(SDL_Surface* pointer_on_blank_square_black_border,
                            SDL_Surface* *pointer_on_wall_square,
                            SDL_Surface* *pointer_on_objective_square,
                            SDL_Surface* *pointer_on_box_square);


/*----------------------------------------
------------------------------------------
----------------MINI MAP------------------
------------------------------------------
----------------------------------------*/


/*------------------------------
----------DRAW MINI MAP---------
------------------------------*/

void draw_mini_map(int starting_y_coordinate, int starting_x_coordinate, int map_data[][12],
                    SDL_Surface* squares[144], SDL_Surface* blank_square_black_border,
                    SDL_Surface* wall_square, SDL_Surface* box_square,
                    SDL_Surface* objective_square, SDL_Surface* window);

/*------------------------------
---SHOW ALL THE WINDOW CONTENT--
------------------------------*/

void show_names_and_minimaps_and_buttons(int map_data[][12],
                                        SDL_Surface* blank_square_black_border,
                                        SDL_Surface* wall_square, SDL_Surface* box_square,
                                        SDL_Surface* objective_square, SDL_Surface* window,
                                        SDL_Surface* select_map_button);


/*----------------------------------------
------------------------------------------
------------SELECT MAP WINDOW-------------
------------------------------------------
----------------------------------------*/


void load_select_map_window(SDL_Surface* window);
