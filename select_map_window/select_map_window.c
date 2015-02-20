#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../tools/write_text_on_window/write_text_on_window.h"
#include "../map_editor/map_editor.h"
#include "../game_window/game_window.h"
#include "../main_window/main_window.h"

/*----------------------------------------
------------------------------------------
---------------MINI SPRITES---------------
------------------------------------------
----------------------------------------*/


void load_mini_sprites(SDL_Surface* *pointer_on_blank_square_black_border,
                            SDL_Surface* *pointer_on_wall_square,
                            SDL_Surface* *pointer_on_objective_square,
                            SDL_Surface* *pointer_on_box_square) {
    *pointer_on_blank_square_black_border = IMG_Load
                                            ("./images/sprites/mini/blank_black_border.jpg");

    *pointer_on_wall_square = IMG_Load("./images/sprites/mini/mur.jpg");

    *pointer_on_objective_square = IMG_Load("./images/sprites/mini/objectif.png");

    *pointer_on_box_square = IMG_Load("./images/sprites/mini/caisse.jpg");

}


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
                    SDL_Surface* objective_square, SDL_Surface* window) {
    int x = 0, y = 0, i = 0;

    SDL_Rect surface_position;

    surface_position.x = starting_x_coordinate;

    // Loading the right images in every square.
    for (x = 0; x < 12; x++) {

        surface_position.y = starting_y_coordinate;

        for (y = 0; y < 12; y++) {
            switch (map_data[x][y]) {
                case 0:
                    squares[i] = blank_square_black_border;
                    break;
                case 1:
                    squares[i] = wall_square;
                    break;
                case 2:
                    squares[i] = box_square;
                    break;
                case 3:
                    squares[i] = objective_square;
                    break;
            }

            // We make the square appear on the window.
            SDL_BlitSurface(squares[i], NULL, window, &surface_position);

            surface_position.y += 10; // 10 is the height in pixel of the mini sprites.

            i++; // "i" should get bigger until it reaches the total number of squares surfaces.
        }

        surface_position.x += 10;
    }
}

/*------------------------------
---SHOW ALL THE WINDOW CONTENT--
------------------------------*/

void show_window_contents(int map_data[][12],
                                        SDL_Surface* blank_square_black_border,
                                        SDL_Surface* wall_square, SDL_Surface* box_square,
                                        SDL_Surface* objective_square, SDL_Surface* window,
                                        SDL_Surface* select_map_button,
                                        SDL_Surface* blue_scrollbar, int page_number) {
    int i = 0, j = 0;

    char map_name[100] = "";

    SDL_Surface *squares[144] = {NULL};

    for (i = page_number; i < page_number + 3; i++) {
        sprintf(map_name, "./maps/map%d.map", i);
        load_map(map_name, map_data);
        sprintf(map_name, "Map %d", i);

        draw_mini_map(10 + (i * 130), 75, map_data, squares, blank_square_black_border,
                    wall_square, box_square, objective_square, window);

        blit_surface(window, select_map_button, 225, 60 + (j*130));

        write_text_on_window(window, 10, 65 + (j * 130), 15, 0, 0, 0, map_name);

        printf("tour---");
        j++;
    }

    blit_surface(window, blue_scrollbar, 355, 10);

    SDL_Flip(window);

}


/*----------------------------------------
------------------------------------------
------------SELECT MAP WINDOW-------------
------------------------------------------
----------------------------------------*/


void load_select_map_window(SDL_Surface* window) {
    int continue_loop = 1;

    int number_of_pages = ((get_number_of_maps() - 1) / 3) + 1;

    SDL_Surface *blank_square_black_border = NULL, *wall_square = NULL,
                *objective_square = NULL, *box_square = NULL, *select_map_button = NULL,
                *blue_scrollbar = NULL;

    int map_data[12][12];

    SDL_Event event;

    int selected_map_nb = -1;

    //int number_of_map_pages = get_number_of_maps() / 3;

    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    load_mini_sprites(&blank_square_black_border, &wall_square, &objective_square, &box_square);

    select_map_button = IMG_Load("./images/buttons/select_map.png");
    blue_scrollbar = IMG_Load("./images/blue_scrollbar.png");

    show_window_contents(map_data, blank_square_black_border, wall_square, box_square,
                            objective_square, window, select_map_button, blue_scrollbar, 1);

    while (continue_loop)
    {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                SDL_FreeSurface(blank_square_black_border);
                SDL_FreeSurface(wall_square);
                SDL_FreeSurface(objective_square);
                SDL_FreeSurface(box_square);
                SDL_FreeSurface(select_map_button);
                SDL_FreeSurface(blue_scrollbar);

                continue_loop = 0;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // If we click on the first select map button.
                    if ((event.button.x >= 225 && event.button.y >= 60)
                        && (event.button.x <= 346 && event.button.y <= 89)) {
                        selected_map_nb = 0;
                    }

                    // If we click on the second select map button.
                    if ((event.button.x >= 225 && event.button.y >= 190)
                        && (event.button.x <= 346 && event.button.y <= 219)) {
                        selected_map_nb = 1;
                    }

                    // If we click on the third select map button.
                    if ((event.button.x >= 225 && event.button.y >= 320)
                        && (event.button.x <= 346 && event.button.y <= 349)) {
                        selected_map_nb = 2;
                    }

                     // If we click on the top button of the scroll bar.
                    if ((event.button.x >= 355 && event.button.y >= 10)
                            && (event.button.x <= 397 && event.button.y <= 65)) {
                    }

                     // If we click on the bottom button of the scroll bar.
                    if ((event.button.x >= 355 && event.button.y >= 333)
                            && (event.button.x <= 397 && event.button.y <= 388)) {
                    }

                    // If a map has been selected, we return to the main window.
                    if (selected_map_nb != -1) {
                        SDL_FreeSurface(blank_square_black_border);
                        SDL_FreeSurface(wall_square);
                        SDL_FreeSurface(objective_square);
                        SDL_FreeSurface(box_square);
                        SDL_FreeSurface(select_map_button);

                        load_main_window(window, selected_map_nb);

                        return; // Leaving the select map window function.
                    }
                }
        }
    }
}
