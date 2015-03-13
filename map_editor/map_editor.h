/*----------------------------------------
------------------------------------------
-------------MAP EDITOR WINDOW------------
------------------------------------------
----------------------------------------*/


void load_map_editor();


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
---------------DRAW MAP GRID--------------
------------------------------------------
----------------------------------------*/


// Draw all the black border blank squares to form the map grid.
void draw_map_grid(SDL_Surface* blank_square_black_border, SDL_Surface* window, int map_height,
                   int map_width);


/*----------------------------------------
------------------------------------------
------------------SPRITES-----------------
------------------------------------------
----------------------------------------*/

/*-------------------------------------------
--------------SET XY POS AND BLIT------------
-------------------------------------------*/

// Useful function for the function below.
void set_xy_pos_and_blit(SDL_Rect* sprite_position, int x, int y, SDL_Surface* window,
                         SDL_Surface* *sprite);

/*-------------------------------------------
------------LOAD AND BLIT SPRITES------------
-------------------------------------------*/

void load_and_blit_map_editor_sprites(SDL_Surface* window,
                                      SDL_Surface* *pointer_on_black_border_square,
                                      SDL_Rect* black_border_square_position,
                                      SDL_Surface* *pointer_on_wall_square,
                                      SDL_Rect *wall_square_position,
                                      SDL_Surface* *pointer_on_objective_square,
                                      SDL_Rect *objective_square_position,
                                      SDL_Surface* *pointer_on_box_square,
                                      SDL_Rect *box_square_position,
                                      SDL_Surface* *pointer_on_mario_sprite,
                                      SDL_Rect *mario_sprite_position,
                                      SDL_Surface* *pointer_on_black_bar,
                                      SDL_Surface* *pointer_on_save_map_button,
                                      SDL_Rect* save_map_button_position,
                                      SDL_Surface* *pointer_on_save_map_button_clicked,
                                      SDL_Surface* *pointer_on_exit_editor_button,
                                      SDL_Rect* exit_button_position);

/*-------------------------------------------
-----------BLIT NEW SELECTED SPRITE----------
-------------------------------------------*/

// Blit the newly selected sprite to show it as the actual selected sprite.
void blit_new_selected_sprite(SDL_Surface* sprite, SDL_Surface* window,
                              SDL_Rect selected_sprite_position);

/*-------------------------------------------
------------CHANGE SELECTED SPRITE-----------
-------------------------------------------*/

// Useful function for the function below (change_selected_sprite) to avoid repetitions.
void check_and_blit_new_selected_sprite(SDL_Event event, SDL_Rect sprite_position,
                                        SDL_Surface* sprite, int* selected_sprite,
                                        int sprite_code, SDL_Surface* window);

// Change the shown selected sprite if the user clicks on a proposed sprite.
void change_selected_sprite(SDL_Surface* window, SDL_Event event,
                            SDL_Surface* blank_square_black_border,
                            SDL_Rect black_border_square_position, SDL_Surface* wall_square,
                            SDL_Rect wall_square_position, SDL_Surface* box_square,
                            SDL_Rect box_square_position, SDL_Surface* objective_square,
                            SDL_Rect objective_square_position, SDL_Surface* mario_sprite,
                            SDL_Rect mario_sprite_position, int* selected_sprite);

/*-------------------------------------------
------BLIT SELECTED SPRITE ON THE GRID-------
-------------------------------------------*/

/* Check if a number is a multiple of 34 in the 0-408 range.
This function is used in the function below (blit_selected_sprite). */
int check_if_number_in_range(int x);

// Blit the selected sprite on the map editor grid.
void blit_selected_sprite(int map_width, int map_height, SDL_Surface* window,
                          SDL_Surface* blank_square_black_border, SDL_Surface* wall_square,
                          SDL_Surface* objective_square, SDL_Surface* box_square,
                          SDL_Surface* mario_sprite, SDL_Event event, int selected_sprite,
                          int map_data[][12], int* mario_has_been_blited);


/*----------------------------------------
------------------------------------------
-----------FREEING SDL SURFACES-----------
------------------------------------------
----------------------------------------*/


void free_sdl_surfaces_map_editor(SDL_Surface* window, SDL_Surface* blank_square_black_border,
                       SDL_Surface* wall_square, SDL_Surface* objective_square,
                       SDL_Surface* box_square, SDL_Surface* mario_sprite, SDL_Surface* black_bar,
                       SDL_Surface* save_map_button, SDL_Surface* save_map_button_clicked,
                       SDL_Surface* exit_editor_button);
