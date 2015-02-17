/*----------------------------------------
------------------------------------------
-----------------MAP DATA-----------------
------------------------------------------
----------------------------------------*/


/*-------------------------------------------
-------GETTING THE NUMBER OF STORED MAPS-----
-------------------------------------------*/

int get_number_of_maps();

/*-------------------------------------------
-FILLING MAP DATA WITH ZEROS (BLANK SQUARES)-
-------------------------------------------*/

void fill_map_with_zeros(int map_data[][12]);

/*-------------------------------------------
-------CLICKING ON THE "SAVE MAP" BUTTON-----
-------------------------------------------*/

void click_on_save_map_button(SDL_Surface* window, SDL_Surface* save_map_button,
                                SDL_Surface* save_map_button_clicked, int map_data[][12]);

/*-------------------------------------------
--------SAVING THE MAP IN AN UNIQUE FILE-----
-------------------------------------------*/

void save_map(int map_data[][12]);


/*----------------------------------------
------------------------------------------
-------------BLITTING SURFACES------------
------------------------------------------
----------------------------------------*/


// Make the blitting of surfaces easier, with less repetitions.
void blit_surface(SDL_Surface* window, SDL_Surface* surface, int x, int y);

// Load and blit the sprites propositions of the right panel.
void load_and_blit_sprite_propositions(SDL_Surface* window, SDL_Surface* blank_square_black_border,
                                        SDL_Surface* *pointer_on_wall_square,
                                        SDL_Surface* *pointer_on_objective_square,
                                        SDL_Surface* *pointer_on_box_square);


/*----------------------------------------
------------------------------------------
--------------SELECTED SPRITE-------------
------------------------------------------
----------------------------------------*/


// Change the shown selected sprite if the user clicks on a proposed sprite.
void change_selected_sprite(SDL_Surface* window, SDL_Event event,
                            SDL_Surface* blank_square_black_border, SDL_Surface* wall_square,
                            SDL_Surface* objective_square, SDL_Surface* box_square,
                            int* selected_sprite);

/* Useful function for the blit_selected_sprite function.
Check if a number is a multiple of 34 in the 0-408 range. */
int check_if_number_in_range(int x);

// Blit the selected sprite on the map editor grid.
void blit_selected_sprite(SDL_Surface* window, SDL_Surface* blank_square_black_border,
                            SDL_Surface* wall_square, SDL_Surface* objective_square,
                            SDL_Surface* box_square, SDL_Event event, int selected_sprite,
                            int map_data[][12]);


/*----------------------------------------
------------------------------------------
-------------MAP EDITOR WINDOW------------
------------------------------------------
----------------------------------------*/


void load_map_editor();
