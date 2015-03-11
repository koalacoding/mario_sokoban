#include <unistd.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "map_editor.h"

#include "../tools/write_text_on_window/write_text_on_window.h"
#include "../tools/blit_surface/blit_surface.h"
#include "../tools/has_surface_been_clicked/has_surface_been_clicked.h"
#include "../main_window/main_window.h"


/*----------------------------------------
------------------------------------------
-------------MAP EDITOR WINDOW------------
------------------------------------------
----------------------------------------*/


void load_map_editor() {
    int window_height, window_width, map_height, map_width;

    SDL_Surface *window, *blank_square_black_border = NULL, *wall_square = NULL,
                *objective_square = NULL, *box_square = NULL, *mario_sprite = NULL,
                *black_bar = NULL, *save_map_button = NULL,
                *save_map_button_clicked = NULL, *exit_editor_button = NULL;

    SDL_Rect black_border_square_position, wall_square_position, objective_square_position,
             save_map_button_position, exit_button_position;

    SDL_Event event;

    int map_data[12][12];

    int x = 0, y = 0;

    int selected_sprite = 0;

    int mario_has_been_blited = 0;

    window_width = 500;
    window_height = 408;

    window = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban : Map editor", NULL);
    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    load_and_blit_map_editor_sprites(window, &blank_square_black_border,
                                     &black_border_square_position, &wall_square,
                                     &wall_square_position, &objective_square,
                                     &objective_square_position, &box_square,
                                     &mario_sprite, &black_bar, &save_map_button,
                                     &save_map_button_position, &save_map_button_clicked,
                                     &exit_editor_button, &exit_button_position);

    // Fill the map data with zeros, as we start with no sprite blited.
    fill_map_with_zeros(map_data);
    map_width = map_height = 408;
    draw_map_grid(blank_square_black_border, window, map_height, map_width);

    write_text_on_window(window, 412, 5, 10, 0, 0, 0, "Selected sprite :");

    while (1)
    {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                free_sdl_surfaces_map_editor(window, blank_square_black_border, wall_square,
                                             objective_square, box_square, mario_sprite, black_bar,
                                             save_map_button, save_map_button_clicked,
                                             exit_editor_button);

                return;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) { // Mouse left click.
                    // If the user clicks on a sprite proposition, we change the selected sprite.
                    change_selected_sprite(window, event, blank_square_black_border,
                                           black_border_square_position, wall_square,
                                           wall_square_position, objective_square, box_square,
                                           mario_sprite, &selected_sprite);

                    // If we click on the map.
                    if (event.button.x < 408 && event.button.y < 408) {
                        /*  If we click on a map square,
                        the square will change to the selected sprite. */
                        blit_selected_sprite(window, blank_square_black_border, wall_square,
                                                objective_square, box_square, mario_sprite, event,
                                                selected_sprite, map_data, &mario_has_been_blited);
                    }

                    // If we click on the save map button.
                    if (has_surface_been_clicked(event.button.x, event.button.y,
                                                 save_map_button_position, save_map_button) == 1) {
                        click_on_save_map_button(window, save_map_button, save_map_button_clicked,
                                                    map_data);
                    }

                    // If we click on the exit button.
                    if (has_surface_been_clicked(event.button.x, event.button.y,
                                                 exit_button_position, exit_editor_button) == 1) {
                        free_sdl_surfaces_map_editor(window, blank_square_black_border, wall_square,
                                                     objective_square, box_square, mario_sprite,
                                                     black_bar, save_map_button,
                                                     save_map_button_clicked, exit_editor_button);

                        return;
                    }
                }

                break;
        }
    }
}


/*----------------------------------------
------------------------------------------
-----------------MAP DATA-----------------
------------------------------------------
----------------------------------------*/


/*-------------------------------------------
-------GETTING THE NUMBER OF STORED MAPS-----
-------------------------------------------*/

int get_number_of_maps() {
    FILE* file;

    char phrase[100] = "";

    int number_of_maps = 0;

    file = fopen("./maps/map_list.txt", "rb");

    if (file != NULL) {
        while (fgets(phrase, 100, file) != NULL) {
            number_of_maps++;
        }
    }

    fclose(file);

    return number_of_maps;
}

/*-------------------------------------------
-FILLING MAP DATA WITH ZEROS (BLANK SQUARES)-
-------------------------------------------*/

void fill_map_with_zeros(int map_data[][12]) {
    int x, y;

    for (x = 0; x < 12; x++) {
        for (y = 0; y < 12; y++) {
            map_data[x][y] = 0;
        }
    }
}

/*-------------------------------------------
-------CLICKING ON THE "SAVE MAP" BUTTON-----
-------------------------------------------*/

void click_on_save_map_button(SDL_Surface* window, SDL_Surface* save_map_button,
                                SDL_Surface* save_map_button_clicked, int map_data[][12]) {
    blit_surface(window, save_map_button_clicked, 409, 310);
    SDL_Flip(window);
    SDL_Delay(200);
    blit_surface(window, save_map_button, 409, 310);
    SDL_Flip(window);

    save_map(map_data);
}

/*-------------------------------------------
--------SAVING THE MAP IN AN UNIQUE FILE-----
-------------------------------------------*/

void save_map(int map_data[][12]) {
    int number_of_maps = get_number_of_maps();

    char new_map_name[20];

    FILE* file;

    int y, x;

    sprintf(new_map_name, "./maps/map%d.map", number_of_maps);

    file = fopen(new_map_name, "wb");

    for (y = 0; y < 12; y++) {
        for (x = 0; x < 12; x++) {
            fprintf(file, "%d ", map_data[x][y]);
        }
        fprintf(file, "\n"); // Jumping to the next line after we finish filling a line.
    }

    fclose(file);

    // Writing the name of the new map into map_list.txt.
    file = fopen("./maps/map_list.txt", "ab");
    fseek(file, 0, SEEK_END); // Going to the end of the list.
    sprintf(new_map_name, "map%d.map\n", number_of_maps);
    fprintf(file, "%s", new_map_name);
    fclose(file);
}


/*----------------------------------------
------------------------------------------
---------------DRAW MAP GRID--------------
------------------------------------------
----------------------------------------*/


// Draw all the black border blank squares to form the map grid.
void draw_map_grid(SDL_Surface* blank_square_black_border, SDL_Surface* window, int map_height,
                   int map_width) {
    int x, y;
    SDL_Rect surface_position;
    surface_position.x = 0; // We don't initialize y because we do it below in the for loop.

    for (x = 0; x < 12; x++) { // Filling the window with black border blank squares.
        // We need to reset surface_position.y to 0 to start at the beginning of a new line.
        surface_position.y = 0;

        for (y = 0; y < 12; y++) {
            SDL_BlitSurface(blank_square_black_border, NULL, window, &surface_position);

            surface_position.y += (map_height / 12); // Going to the next line.
        }

        surface_position.x += (map_width / 12); // Going to the next column.
    }
}


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
                         SDL_Surface* *sprite) {
    sprite_position->x = x;
    sprite_position->y = y;
    blit_surface(window, *sprite, x, y);
}

/*-------------------------------------------
------------LOAD AND BLIT SPRITES------------
-------------------------------------------*/

void load_and_blit_map_editor_sprites(SDL_Surface* window,
                                      SDL_Surface* *pointer_on_black_border_square,
                                      SDL_Rect* black_border_square_position,
                                      SDL_Surface* *pointer_on_wall_square,
                                      SDL_Rect* wall_square_position,
                                      SDL_Surface* *pointer_on_objective_square,
                                      SDL_Rect* objective_square_position,
                                      SDL_Surface* *pointer_on_box_square,
                                      SDL_Surface* *pointer_on_mario_sprite,
                                      SDL_Surface* *pointer_on_black_bar,
                                      SDL_Surface* *pointer_on_save_map_button,
                                      SDL_Rect* save_map_button_position,
                                      SDL_Surface* *pointer_on_save_map_button_clicked,
                                      SDL_Surface* *pointer_on_exit_editor_button,
                                      SDL_Rect* exit_button_position) {
    *pointer_on_black_border_square = IMG_Load("./images/sprites/blank_black_border.jpg");
    // To put it as default selected sprite.
    set_xy_pos_and_blit(black_border_square_position, 435, 25, window,
                        pointer_on_black_border_square);
    // To put it as a sprite proposition.
    set_xy_pos_and_blit(black_border_square_position, 435, 72, window,
                        pointer_on_black_border_square);

    *pointer_on_wall_square = IMG_Load("./images/sprites/mur.jpg");
    set_xy_pos_and_blit(wall_square_position, 435, 122, window,
                        pointer_on_wall_square);

    *pointer_on_objective_square = IMG_Load("./images/sprites/objectif.png");
    set_xy_pos_and_blit(objective_square_position, 435, 172, window,
                        pointer_on_objective_square);

    *pointer_on_box_square = IMG_Load("./images/sprites/caisse.jpg");
    blit_surface(window, *pointer_on_box_square, 435, 222);

    *pointer_on_mario_sprite = IMG_Load("./images/sprites/robot.png");
    blit_surface(window, *pointer_on_mario_sprite, 435, 272);

    *pointer_on_black_bar = IMG_Load("./images/black_bar.png");
    blit_surface(window, *pointer_on_black_bar, 420, 65);

    *pointer_on_save_map_button = IMG_Load("./images/buttons/save_map_button.png");
    save_map_button_position->x = 409;
    save_map_button_position->y = 310;
    blit_surface(window, *pointer_on_save_map_button, save_map_button_position->x,
                 save_map_button_position->y);

    *pointer_on_save_map_button_clicked = IMG_Load("./images/buttons/save_map_button_clicked.png");

    *pointer_on_exit_editor_button = IMG_Load("./images/buttons/exit_button.png");
    exit_button_position->x = 432;
    exit_button_position->y = 360;
    blit_surface(window, *pointer_on_exit_editor_button, exit_button_position->x,
                 exit_button_position->y);

    SDL_Flip(window);
}

/*-------------------------------------------
------------CHANGE SELECTED SPRITE-----------
-------------------------------------------*/

// Function to change the shown selected sprite if the user clicks on a sprite.
void change_selected_sprite(SDL_Surface* window, SDL_Event event,
                            SDL_Surface* blank_square_black_border,
                            SDL_Rect black_border_square_position, SDL_Surface* wall_square,
                            SDL_Rect wall_square_position, SDL_Surface* objective_square,
                            SDL_Surface* box_square, SDL_Surface* mario_sprite,
                            int* selected_sprite) {
    SDL_Rect selected_sprite_position;
    selected_sprite_position.x = 435;
    selected_sprite_position.y = 25;

    // If we click on the blank square sprite.
    if (has_surface_been_clicked(event.button.x, event.button.y, black_border_square_position,
                                 blank_square_black_border) == 1) {
        *selected_sprite = 0;
        blit_new_selected_sprite(blank_square_black_border, window, selected_sprite_position);
    }

    // If we click on the wall sprite.
    if (has_surface_been_clicked(event.button.x, event.button.y, black_border_square_position,
                                 blank_square_black_border) == 1) {
        *selected_sprite = 0;
        blit_new_selected_sprite(blank_square_black_border, window, selected_sprite_position);
    }

    if (event.button.x >= 435 && event.button.x <= 469) {
        // If the user clicks on the wall sprite.
        if (event.button.y >= 122 && event.button.y <= 156) {
            *selected_sprite = 1;

            // We change the selected square to the wall sprite.
            blit_surface(window, wall_square, 435, 25);
            SDL_Flip(window);
        }
        // If the user clicks on the objective sprite.
        else if (event.button.y >= 172 && event.button.y <= 206) {
            *selected_sprite = 2;

            // We change the selected square to the wall sprite.
            blit_surface(window, objective_square, 435, 25);
            SDL_Flip(window);
        }
        // If the user clicks on the box sprite.
        else if (event.button.y >= 222 && event.button.y <= 256) {
            *selected_sprite = 3;

            // We change the selected square to the box sprite.
            blit_surface(window, box_square, 435, 25);
            SDL_Flip(window);
        }
        // If the user clicks on the box sprite.
        else if (event.button.y >= 272 && event.button.y <= 306) {
            *selected_sprite = 4;

            // We change the selected square to the box sprite.
            blit_surface(window, mario_sprite, 435, 25);
            SDL_Flip(window);
        }
    }
}

/*-------------------------------------------
-----------BLIT NEW SELECTED SPRITE----------
-------------------------------------------*/

void blit_new_selected_sprite(SDL_Surface* sprite, SDL_Surface* window,
                              SDL_Rect selected_sprite_position) {
    SDL_BlitSurface(sprite, NULL, window, &selected_sprite_position);
    SDL_Flip(window);
}

/*-------------------------------------------
-----------CHECK IF NUMBER IN RANGE----------
-------------------------------------------*/

/* Check if a number is a multiple of 34 in the 0-408 range.
This function is used in the function below (blit_selected_sprite). */

int check_if_number_in_range(int x) {
    int i = 0, j = 0;

    for (i = 0; i <= 408; i++) {
        if (x == j) {
            return 1;
        }

        j += 34;
    }

    return 0;
}

void blit_selected_sprite(SDL_Surface* window, SDL_Surface* blank_square_black_border,
                            SDL_Surface* wall_square, SDL_Surface* objective_square,
                            SDL_Surface* box_square, SDL_Surface* mario_sprite, SDL_Event event,
                            int selected_sprite, int map_data[][12], int* mario_has_been_blited) {
    // Ajusting the x and y coordinates to allow the sprite to fit exactly in the square.

    while (check_if_number_in_range(event.button.x) != 1) {
        event.button.x--;
    }

    while (check_if_number_in_range(event.button.y) != 1) {
        event.button.y--;
    }

    // If we blit the selected sprite on a square where we already blited Mario.
    if (map_data[(event.button.x / 34)][(event.button.y / 34)] == 5) {
        *mario_has_been_blited = 0;
    }

    switch (selected_sprite) {
        case 0: // Blank sprite.
            map_data[(event.button.x / 34)][(event.button.y / 34)] = 0;
            blit_surface(window, blank_square_black_border, event.button.x, event.button.y);
            break;
        case 1: // Wall sprite.
            map_data[(event.button.x / 34)][(event.button.y / 34)] = 1;
            blit_surface(window, wall_square, event.button.x, event.button.y);
            break;
        case 2: // Objective sprite.
            map_data[(event.button.x / 34)][(event.button.y / 34)] = 3;
            blit_surface(window, objective_square, event.button.x, event.button.y);
            break;
        case 3: // Box sprite.
            map_data[(event.button.x / 34)][(event.button.y / 34)] = 2;
            blit_surface(window, box_square, event.button.x, event.button.y);
            break;
        case 4: // Mario sprite.
            if (*mario_has_been_blited == 0) {
                map_data[(event.button.x / 34)][(event.button.y / 34)] = 5;
                blit_surface(window, mario_sprite, event.button.x, event.button.y);

                *mario_has_been_blited = 1;
            }

            break;
    }

    SDL_Flip(window);
}


/*----------------------------------------
------------------------------------------
-----------FREEING SDL SURFACES-----------
------------------------------------------
----------------------------------------*/


void free_sdl_surfaces_map_editor(SDL_Surface* window, SDL_Surface* blank_square_black_border,
                       SDL_Surface* wall_square, SDL_Surface* objective_square,
                       SDL_Surface* box_square, SDL_Surface* mario_sprite, SDL_Surface* black_bar,
                       SDL_Surface* save_map_button, SDL_Surface* save_map_button_clicked,
                       SDL_Surface* exit_editor_button) {
    SDL_FreeSurface(window);
    SDL_FreeSurface(blank_square_black_border);
    SDL_FreeSurface(wall_square);
    SDL_FreeSurface(objective_square);
    SDL_FreeSurface(box_square);
    SDL_FreeSurface(mario_sprite);
    SDL_FreeSurface(black_bar);
    SDL_FreeSurface(save_map_button);
    SDL_FreeSurface(save_map_button_clicked);
    SDL_FreeSurface(exit_editor_button);
}
