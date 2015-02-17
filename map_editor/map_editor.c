#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <unistd.h>
#include <stdio.h>
#include "../tools/write_text_on_window/write_text_on_window.h"
#include "map_editor.h"
#include "../main_window/main_window.h"
#include "../game_window/game_window.h"

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

    file = fopen("maps/map_list.txt", "rb");

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
    int x = 0, y = 0;

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

    sprintf(new_map_name, "maps/map%d.map", number_of_maps);

    FILE* file;

    file = fopen(new_map_name, "wb");

    int y = 0, x = 0;

    for (y = 0; y < 12; y++) {
        for (x = 0; x < 12; x++) {
            fprintf(file, "%d ", map_data[x][y]);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    // Writing the name of the new map into map_list.txt.

    file = fopen("maps/map_list.txt", "ab");

    fseek(file, 0, SEEK_END); // Going to the end of the list.

    sprintf(new_map_name, "\nmap%d.map", number_of_maps);
    fprintf(file, "%s", new_map_name);

    fclose(file);
}

/*----------------------------------------
------------------------------------------
-------------BLITTING SURFACES------------
------------------------------------------
----------------------------------------*/


// Avoiding some repetitions with this function.
void blit_surface(SDL_Surface* window, SDL_Surface* surface, int x, int y) {

    SDL_Rect surface_position;

    surface_position.x = x;
    surface_position.y = y;
    SDL_BlitSurface(surface, NULL, window, &surface_position);
}

// Load and blit the sprites propositions of the right panel.
void load_and_blit_sprite_propositions(SDL_Surface* window, SDL_Surface* blank_square_black_border,
                                        SDL_Surface* *pointer_on_wall_square,
                                        SDL_Surface* *pointer_on_objective_square,
                                        SDL_Surface* *pointer_on_box_square) {
    blit_surface(window, blank_square_black_border, 435, 110);

    *pointer_on_wall_square = IMG_Load("./images/sprites/mur.jpg");
    blit_surface(window, *pointer_on_wall_square, 435, 160);

    *pointer_on_objective_square = IMG_Load("./images/sprites/objectif.png");
    blit_surface(window, *pointer_on_objective_square, 435, 210);

    *pointer_on_box_square = IMG_Load("./images/sprites/caisse.jpg");
    blit_surface(window, *pointer_on_box_square, 435, 260);

}


/*----------------------------------------
------------------------------------------
--------------SELECTED SPRITE-------------
------------------------------------------
----------------------------------------*/


// Function to change the shown selected sprite if the user clicks on a sprite.
void change_selected_sprite(SDL_Surface* window, SDL_Event event,
                            SDL_Surface* blank_square_black_border, SDL_Surface* wall_square,
                            SDL_Surface* objective_square, SDL_Surface* box_square,
                            int* selected_sprite) {

    if (event.button.x >= 435 && event.button.x <= 469) {
        // If the user clicks on the blank square sprite.
        if (event.button.y >= 110 && event.button.y <= 144) {
            *selected_sprite = 0;

            // We change the selected square to the blank square sprite.
            blit_surface(window, blank_square_black_border, 435, 40);
            SDL_Flip(window);
        }
        // If the user clicks on the wall sprite.
        else if (event.button.y >= 160 && event.button.y <= 194) {
            *selected_sprite = 1;

            // We change the selected square to the wall sprite.
            blit_surface(window, wall_square, 435, 40);
            SDL_Flip(window);
        }
        // If the user clicks on the objective sprite.
        else if (event.button.y >= 210 && event.button.y <= 244) {
            *selected_sprite = 2;

            // We change the selected square to the wall sprite.
            blit_surface(window, objective_square, 435, 40);
            SDL_Flip(window);
        }
        // If the user clicks on the box sprite.
        else if (event.button.y >= 260 && event.button.y <= 294) {
            *selected_sprite = 3;

            // We change the selected square to the box sprite.
            blit_surface(window, box_square, 435, 40);
            SDL_Flip(window);
        }
    }
}

// Check if a number is a multiple of 34 in the 0-408 range.
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
                            SDL_Surface* box_square, SDL_Event event, int selected_sprite,
                            int map_data[][12]) {
    // Ajusting the x and y coordinates to allow the sprite to fit exactly in the square.

    while (check_if_number_in_range(event.button.x) != 1) {
        event.button.x--;
    }

    while (check_if_number_in_range(event.button.y) != 1) {
        event.button.y--;
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
    }

    SDL_Flip(window);
}


/*----------------------------------------
------------------------------------------
-------------MAP EDITOR WINDOW------------
------------------------------------------
----------------------------------------*/


void load_map_editor() {
    int window_height, window_width;

    SDL_Surface *window, *blank_square_black_border = NULL, *wall_square = NULL,
                *objective_square = NULL, *box_square = NULL, *black_bar = NULL,
                *save_map_button = NULL, *save_map_button_clicked = NULL,
                *exit_editor_button = NULL;

    SDL_Rect surface_position;

    SDL_Event event;

    int map_data[12][12];

    int x = 0, y = 0;

    int continue_loop = 1;

    int selected_sprite = 0;

    window = SDL_SetVideoMode(500, 408, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    window_width = 408;
    window_height = 408;

    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    blank_square_black_border = IMG_Load("./images/sprites/blank_black_border.jpg");

    fill_map_with_zeros(map_data);

    surface_position.x = 0; // Initialization.

    for (x = 0; x < 12; x++) { // Filling the window with black border blank squares.
        // We need to reset surface_position.y to 0 to start at the beginning of the new line.
        surface_position.y = 0;

        for (y = 0; y < 12; y++) {
            SDL_BlitSurface(blank_square_black_border, NULL, window, &surface_position);

            surface_position.y += (window_height / 12); // Going to the next square on the y axis.
        }
        // Through each loop, we add to the x coordinate 1/12 of the window width.
        surface_position.x += (window_width / 12);
    }

    write_text_on_window(window, 412, 20, 10, 0, 0, 0, "Selected sprite :");

    // Default selected sprite is black border blank square.
    blit_surface(window, blank_square_black_border, 435, 40);

    // Blitting a black bar for separation.
    black_bar = IMG_Load("./images/black_bar.png");
    blit_surface(window, black_bar, 420, 90);

    load_and_blit_sprite_propositions(window, blank_square_black_border, &wall_square,
                                        &objective_square, &box_square);

    save_map_button = IMG_Load("./images/buttons/save_map_button.png");
    save_map_button_clicked = IMG_Load("./images/buttons/save_map_button_clicked.png");
    blit_surface(window, save_map_button, 409, 310);

    exit_editor_button = IMG_Load("./images/buttons/exit_button.png");
    blit_surface(window, exit_editor_button, 432, 360);

    SDL_Flip(window);

    while (continue_loop)
    {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                SDL_FreeSurface(blank_square_black_border);
                SDL_FreeSurface(wall_square);
                SDL_FreeSurface(objective_square);
                SDL_FreeSurface(box_square);
                SDL_FreeSurface(save_map_button);
                SDL_FreeSurface(save_map_button_clicked);

                continue_loop = 0;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) { // Mouse left click.
                    // If the user clicks on a sprite proposition, we change the selected sprite.
                    change_selected_sprite(window, event, blank_square_black_border, wall_square,
                                            objective_square, box_square, &selected_sprite);

                    // If we click on the map.
                    if (event.button.x < 408 && event.button.y < 408) {
                        /*  If we click on a map square,
                        the square will change to the selected sprite. */
                        blit_selected_sprite(window, blank_square_black_border, wall_square,
                                                objective_square, box_square, event,
                                                selected_sprite, map_data);
                    }

                    // If we click on the save map button.
                    if ((event.button.x >= 409 && event.button.y >= 310)
                        && (event.button.x <= 499 && event.button.y <= 341)) {
                        click_on_save_map_button(window, save_map_button, save_map_button_clicked,
                                                    map_data);
                    }

                    // If we click on the exit button.
                    if ((event.button.x >= 432 && event.button.y >= 360)
                        && (event.button.x <= 477 && event.button.y <= 388)) {
                        SDL_FreeSurface(blank_square_black_border);
                        SDL_FreeSurface(wall_square);
                        SDL_FreeSurface(objective_square);
                        SDL_FreeSurface(box_square);
                        SDL_FreeSurface(save_map_button);
                        SDL_FreeSurface(save_map_button_clicked);

                        continue_loop = 0;

                        load_main_window(window);
                    }
                }

                break;
        }
    }
}
