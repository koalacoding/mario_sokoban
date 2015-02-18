#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "../tools/write_text_on_window/write_text_on_window.h"
#include "../map_editor/map_editor.h"
#include "../game_window/game_window.h"

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


/*----------------------------------------
------------------------------------------
------------SELECT MAP WINDOW-------------
------------------------------------------
----------------------------------------*/


void load_select_map_window(SDL_Surface* window) {
    int i = 0, continue_loop = 1;

    int number_of_maps = get_number_of_maps();

    char map_name[100] = "";

    int map_data[12][12];

    SDL_Surface *squares[144] = {NULL}, *blank_square_black_border = NULL, *wall_square = NULL,
                *objective_square = NULL, *box_square = NULL;

    SDL_Event event;

    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    load_mini_sprites(&blank_square_black_border, &wall_square, &objective_square, &box_square);

    for (i = 0; i < 3; i++) {
        sprintf(map_name, "./maps/map%d.map", i);
        load_map(map_name, map_data);
        sprintf(map_name, "Map %d", i);
        draw_mini_map(10 + (i * 130), 75, map_data, squares, blank_square_black_border,
                    wall_square, box_square, objective_square, window);
        write_text_on_window(window, 10, 65 + (i * 130), 15, 0, 0, 0, map_name);

    }

    while (continue_loop)
    {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                continue_loop = 0;
                break;
        }
    }
}
