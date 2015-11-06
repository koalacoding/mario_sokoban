#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "game_window.h"

#include "../main_window/main_window.h"
#include "../tools/blit_surface/blit_surface.h"
#include "../tools/has_surface_been_clicked/has_surface_been_clicked.h"

/*------------------------------------------
--------------------------------------------
--------------LOADING THE GAME--------------
--------------------------------------------
------------------------------------------*/

void load_game(int map_number) {
    int window_width, window_height;
    int game_window_width, game_window_height;

    SDL_Surface *window = NULL, *black_bar_vertical = NULL, *exit_button = NULL, *squares[144] = {NULL},
                *blank_square = NULL, *wall_square = NULL, *objective_square = NULL,
                *box_square = NULL, *placed_box_surface = NULL, *mario_surface = NULL;

    // Will contain x and y positions to place the surfaces containing images.
    SDL_Rect mario_xy, exit_button_position;

    /* Example for mario_square_nb below : if Mario is in the second line of the third column,
    these coordinates will be : (1, 2) */
    MarioSquarePosition mario_square_nb;

    int map_data[12][12]; // This 2d array will contain the map of the game level.
    char map_filename[100] = "";

    int number_of_boxes = 0;

    SDL_Event event;

    int number_of_placed_boxes = 0;

    window_width = 508;
    window_height = 408;
    game_window_width = game_window_height = 408;
    window = SDL_SetVideoMode(window_width, window_height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban : Game", NULL);
    // Filling the window with white.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
    exit_button_position.x = 440;
    exit_button_position.y = 20;
    load_and_blit_window_design(window, &black_bar_vertical, &exit_button, game_window_width,
                                exit_button_position);

    /* Putting into the string map_filename the path of the map,
    using the map's number given in the parameter of the load_game's function. */
    sprintf(map_filename, "./maps/map%d.map", map_number);
    load_map(map_filename, map_data); // Loading the data of the map.
    load_game_sprites(&blank_square, &mario_surface, &wall_square, &objective_square, &box_square,
                        &placed_box_surface);
    draw_map(map_data, window, squares, wall_square, box_square, objective_square,
                mario_surface, &number_of_boxes, &mario_xy, &mario_square_nb, game_window_width,
                game_window_height);

    SDL_EnableKeyRepeat(10, 150);

    while (1) {
        SDL_WaitEvent(&event);
        switch(event.type) {
            case SDL_QUIT:
                close_game_window(black_bar_vertical, exit_button, blank_square, wall_square,
                                    objective_square, box_square, placed_box_surface,
                                    mario_surface, window);
                return;
            case SDL_KEYDOWN: // Cases when we press a key.
                switch(event.key.keysym.sym) {
                     case SDLK_UP:
                        move_(0, map_data, &mario_square_nb, blank_square, window, &mario_xy,
                                game_window_width, game_window_height, mario_surface, box_square,
                                placed_box_surface, &number_of_placed_boxes);
                        break;

                    case SDLK_DOWN:
                        move_(1, map_data, &mario_square_nb, blank_square, window, &mario_xy,
                                game_window_width, game_window_height, mario_surface, box_square,
                                placed_box_surface, &number_of_placed_boxes);
                        break;

                    case SDLK_RIGHT:
                        move_(2, map_data, &mario_square_nb, blank_square, window, &mario_xy,
                                game_window_width, game_window_height, mario_surface, box_square,
                                placed_box_surface, &number_of_placed_boxes);
                        break;

                    case SDLK_LEFT:
                        move_(3, map_data, &mario_square_nb, blank_square, window, &mario_xy,
                                game_window_width, game_window_height, mario_surface, box_square,
                                placed_box_surface, &number_of_placed_boxes);

                        break;
                }

                if (have_all_boxes_been_placed(number_of_boxes, number_of_placed_boxes) == 1) {
                    // 500ms delay to see a short moment the map with all the boxes placed.
                    SDL_Delay(500);
                    close_game_window(black_bar_vertical, exit_button, blank_square,
                                        wall_square, objective_square, box_square,
                                        placed_box_surface, mario_surface, window);
                    return;
                }

                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {                     
                    // If we click on the exit button.
                    if (has_surface_been_clicked(event.button.x, event.button.y,
                                                 exit_button_position, exit_button) == 1) {
                        close_game_window(black_bar_vertical, exit_button, blank_square,
                                            wall_square, objective_square, box_square,
                                            placed_box_surface, mario_surface, window);
                        return;
                    }
                }

                break;
        }
    }
}

/*----------------------------------------
------------------------------------------
--------LOAD AND BLIT WINDOW DESIGN-------
------------------------------------------
----------------------------------------*/


void load_and_blit_window_design(SDL_Surface* window,
                                    SDL_Surface* *pointer_on_black_bar_vertical,
                                    SDL_Surface* *pointer_on_exit_button,
                                    int game_window_width,
                                    SDL_Rect exit_button_position) {
    // We have to use pointers here to make IMG_Load work.
    *pointer_on_black_bar_vertical = IMG_Load("./images/black_bar_vertical.png");
    blit_surface(window, *pointer_on_black_bar_vertical, game_window_width + 7, 0);

    *pointer_on_exit_button = IMG_Load("./images/buttons/exit_button.png");
    SDL_BlitSurface(*pointer_on_exit_button, NULL, window, &exit_button_position);
}


/*----------------------------------------
------------------------------------------
-----------------THE MAP------------------
------------------------------------------
----------------------------------------*/


/*---------------------------------
----------LOADING THE MAP----------
---------------------------------*/

int load_map(char* filename, int map_data[][12]) {
    FILE* myfile;

    myfile = fopen(filename, "rb");

    if (myfile == NULL) {
        return 1;
    }

    int y = 0, x = 0;

    for (y = 0; y < 12; y++) {
        for (x = 0; x < 12; x++) {
            fscanf(myfile, "%d", &map_data[x][y]);
        }
    }

    fclose(myfile);

    return 0;
}

/*------------------------------
----------DRAWING THE MAP-------
------------------------------*/

void draw_map(int map_data[][12], SDL_Surface* window, SDL_Surface* squares[144],
                SDL_Surface* wall_surface, SDL_Surface* box_surface,
                SDL_Surface* objective_surface, SDL_Surface* mario_surface, int* number_of_boxes,
                SDL_Rect* mario_xy, MarioSquarePosition* mario_square_nb, int game_window_width,
                int game_window_height) {
    int x = 0, y = 0, i = 0;

    SDL_Rect surface_position;
    surface_position.x = 0;

    // Loading the right images in every square.
    for (x = 0; x < 12; x++) {
        // We need to reset surface_position.y at 0 after each y coordinate for loop.
        surface_position.y = 0;

        for (y = 0; y < 12; y++) {
            switch (map_data[x][y]) {
                case 1:
                    squares[i] = wall_surface;
                    break;
                case 2:
                    squares[i] = box_surface;
                    (*number_of_boxes)++;
                    break;
                case 3:
                    squares[i] = objective_surface;
                    break;
                case 5:
                    squares[i] = mario_surface;
                    mario_xy->x = (game_window_width / 12) * x;
                    mario_xy->y = (game_window_height / 12) * y;
                    mario_square_nb->x = x;
                    mario_square_nb->y = y;

                    // Deleting the Mario starting point in the map data so Mario can walk on it.
                    map_data[x][y] = 0;

                    break;
            }

            // We blit the square on the window.
            SDL_BlitSurface(squares[i], NULL, window, &surface_position);

            surface_position.y += (game_window_height / 12); // Going to the next line of squares.

            // "i" will get bigger until it reaches the total number of squares surfaces (12 x 12, so 144).
            i++;
        }

        // Going to the next square of the line (+1/12 of the line's width)
        surface_position.x += (game_window_width / 12);
    }

    SDL_Flip(window); // Mandatory to refresh the window.
}


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
                        SDL_Surface* *pointer_on_placed_box_sprite) {
    *pointer_on_blank_sprite = IMG_Load("./images/sprites/blank.jpg");
    *pointer_on_mario_sprite = IMG_Load("./images/sprites/robot.png");
    *pointer_on_wall_sprite = IMG_Load("./images/sprites/mur.jpg");
    *pointer_on_objective_sprite = IMG_Load("./images/sprites/objectif.png");
    *pointer_on_box_sprite = IMG_Load("./images/sprites/caisse.jpg");
    *pointer_on_placed_box_sprite = IMG_Load("./images/sprites/caisse_ok.jpg");
}


/*----------------------------------------
------------------------------------------
--------------OBJECT MOVEMENTS------------
------------------------------------------
----------------------------------------*/


/*------------------------------
---------CAN OBJECT MOVE ?------
------------------------------*/

int can_move_object(int mode, int object_square_nb_x, int object_square_nb_y, int is_object_box,
                        int next_square_type) {
    // Mode : 0 = up, 1 = down, 2 = right, 3 = left.
	if ((mode == 0 && object_square_nb_y == 0)
		 || (mode == 1 && object_square_nb_y == 11)
		 || (mode == 2 && object_square_nb_x == 11)
		 || (mode == 3 && object_square_nb_x == 0)) {
		return 0;
	}

    if (is_object_box != 1) { // If the object is not a box (if it is Mario).
        if (next_square_type == 3) { // If the next square is an objective, he can't move on it.
            return 0;
        }
    }

    // If the next square is not a blank square or an objective.
    if (next_square_type != 0 && next_square_type != 3) {
        return 0;
    }

	return 1;
}

/*------------------------------
-----------MOVE OBJECT----------
------------------------------*/

void move_(int mode, int map_data[][12], MarioSquarePosition* mario_square_nb,
            SDL_Surface* blank_square, SDL_Surface* window, SDL_Rect* mario_xy,
            int game_window_width, int game_window_height,
            SDL_Surface* mario_surface, SDL_Surface* box_surface, SDL_Surface* placed_box_surface,
            int* number_of_placed_boxes) {
    int mario_square_nb_x_clone = mario_square_nb->x;
    int mario_square_nb_y_clone = mario_square_nb->y;
    int mario_square_nb_x_clone2 = mario_square_nb_x_clone;
    int mario_square_nb_y_clone2 = mario_square_nb_y_clone;

    switch (mode) {
        case 0: // Up.
            mario_square_nb_y_clone -= 1;
            break;
        case 1: // Down.
            mario_square_nb_y_clone += 1;
            break;
        case 2: // Right.
            mario_square_nb_x_clone += 1;
            break;
        case 3: // Left.
            mario_square_nb_x_clone -= 1;
            break;
    }

    // If there is a box in the direction Mario wants to go.
    if (map_data[mario_square_nb_x_clone][mario_square_nb_y_clone] == 2) {
        /* Here we use clone2 with a jump of 2 squares to test
        what is the square next to the box (2 squares away from Mario).
           [MARIO]------>[BOX]------>[UNKNOWN]
         [MARIO_POS]->[MARIO_POS +1]->[MARIO_POS +2]*/
        switch (mode) {
            case 0: // Up.
                mario_square_nb_y_clone2 -= 2;
                break;
            case 1: // Down.
                mario_square_nb_y_clone2 += 2;
                break;
            case 2: // Right.
                mario_square_nb_x_clone2 += 2;
                break;
            case 3: // Left.
                mario_square_nb_x_clone2 -= 2;
                break;
        }

        // If we can move the box.
        if (can_move_object(mode, mario_square_nb_x_clone, mario_square_nb_y_clone, 1,
            map_data[mario_square_nb_x_clone2][mario_square_nb_y_clone2]) == 1) {
            // Then we move it.
            move_object(blank_square, window, mario_xy, 0, mode, game_window_width,
                        game_window_height, mario_square_nb, mario_square_nb_x_clone,
                        mario_square_nb_y_clone, map_data, mario_surface, box_surface,
                        placed_box_surface, number_of_placed_boxes);

            // And then we move Mario up.
            move_object(blank_square, window, mario_xy, 1, mode, game_window_width,
                        game_window_height, mario_square_nb, mario_square_nb->x,
                        mario_square_nb->y, map_data, mario_surface, box_surface,
                        placed_box_surface, number_of_placed_boxes);
        }
    }

    else { // If the square Mario's direction is not a box.
        // If we can move Mario.
        if (can_move_object(mode, mario_square_nb->x, mario_square_nb->y, 0,
            map_data[mario_square_nb_x_clone][mario_square_nb_y_clone]) == 1) {
            // Then we move Mario up.
            move_object(blank_square, window, mario_xy, 1, mode, game_window_width,
                        game_window_height, mario_square_nb, mario_square_nb->x,
                        mario_square_nb->y, map_data, mario_surface, box_surface,
                        placed_box_surface, number_of_placed_boxes);
        }
    }
}

void move_object(SDL_Surface* blank_square, SDL_Surface* window, SDL_Rect* object_position,
                    int is_object_mario, int mode, int game_window_width, int game_window_height,
                    MarioSquarePosition* mario_square_nb, int object_square_nb_x,
                    int object_square_nb_y, int map_data[][12], SDL_Surface* mario_surface,
                    SDL_Surface* box_surface, SDL_Surface* placed_box_surface,
                    int* number_of_placed_boxes) {
    SDL_Rect object_position_clone;

    int is_box_placed = 0; // If the box is moved on a objective, it will be equal to 1.

    MarioSquarePosition box_square_nb;

    object_position_clone.x = object_position->x;
    object_position_clone.y = object_position->y;

   // We blit a blank square to the object's old position.
    SDL_BlitSurface(blank_square, NULL, window, object_position);

    switch (mode) {
        case 0: // Up.
            if (is_object_mario == 1) {
                object_position->y -= (game_window_height / 12);
                mario_square_nb->y -= 1;
            }

            else { // If the object is a box.
                // If the box is moved on an objective.
                if (map_data[object_square_nb_x][object_square_nb_y - 1] == 3) {
                    is_box_placed = 1;
                }

                // We use the clone to save the right coordinates of the box's new position.
                object_position_clone.y -= ((game_window_height / 12) * 2);
                // We refresh the position of the box in the map data.
                map_data[object_square_nb_x][object_square_nb_y] = 0;
                map_data[object_square_nb_x][object_square_nb_y - 1] = 2;

                if (is_box_placed) {
                    box_square_nb.x = object_square_nb_x;
                    box_square_nb.y = object_square_nb_y - 1;
                    transform_box(map_data, window, placed_box_surface, object_position_clone, box_square_nb,
                                  number_of_placed_boxes);
                    return;
                }
            }

            break;

        case 1: // Down.
            if (is_object_mario == 1) {
                object_position->y += (game_window_height / 12);
                mario_square_nb->y += 1;
            }

            else {
                // If the box is moved on an objective.
                if (map_data[object_square_nb_x][object_square_nb_y + 1] == 3) {
                    is_box_placed = 1;
                }

                object_position_clone.y += ((game_window_height / 12) * 2);
                map_data[object_square_nb_x][object_square_nb_y] = 0;
                map_data[object_square_nb_x][object_square_nb_y + 1] = 2;

                if (is_box_placed) {
                    box_square_nb.x = object_square_nb_x;
                    box_square_nb.y = object_square_nb_y + 1;
                    transform_box(map_data, window, placed_box_surface, object_position_clone, box_square_nb,
                                  number_of_placed_boxes);
                    return;
                }
            }

            break;

        case 2: // Right.
            if (is_object_mario == 1) {
                object_position->x += (game_window_width / 12);
                mario_square_nb->x += 1;
            }

            else {
                // If the box is moved on an objective.
                if (map_data[object_square_nb_x + 1][object_square_nb_y] == 3) {
                    is_box_placed = 1;
                }

                object_position_clone.x += ((game_window_width / 12) * 2);
                map_data[object_square_nb_x][object_square_nb_y] = 0;
                map_data[object_square_nb_x + 1][object_square_nb_y] = 2;

                if (is_box_placed) {
                    box_square_nb.x = object_square_nb_x + 1;
                    box_square_nb.y = object_square_nb_y;
                    transform_box(map_data, window, placed_box_surface, object_position_clone, box_square_nb,
                                  number_of_placed_boxes);
                    return;
                }
            }

            break;
        case 3: // Left.
            if (is_object_mario == 1) {
                object_position->x -= (game_window_width / 12);
                mario_square_nb->x -= 1;
            }

            else {
                // If the box is moved on an objective.
                if (map_data[object_square_nb_x - 1][object_square_nb_y] == 3) {
                    is_box_placed = 1;
                }

                object_position_clone.x -= ((game_window_width / 12) * 2);
                map_data[object_square_nb_x][object_square_nb_y] = 0;
                map_data[object_square_nb_x - 1][object_square_nb_y] = 2;

                if (is_box_placed) {
                    box_square_nb.x = object_square_nb_x - 1;
                    box_square_nb.y = object_square_nb_y;
                    transform_box(map_data, window, placed_box_surface, object_position_clone, box_square_nb,
                                  number_of_placed_boxes);
                    return;
                }
            }

            break;
    }

    // We blit the object at its new position.
    if (is_object_mario == 1) {
        SDL_BlitSurface(mario_surface, NULL, window, object_position);
    }

    else {
        SDL_BlitSurface(box_surface, NULL, window, &object_position_clone);
    }

    SDL_Flip(window); // We refresh the window.
}

/*----------------------------------------
------------------------------------------
-------------------BOXES------------------
------------------------------------------
----------------------------------------*/


/*------------------------------
--TRANSFORM BOX TO PLACED BOX---
------------------------------*/

void transform_box(int map_data[][12], SDL_Surface* window, SDL_Surface* placed_box_surface,
                    SDL_Rect box_position, MarioSquarePosition box_square_nb,
                    int* number_of_placed_boxes) {
    *number_of_placed_boxes += 1;

    // We update the map data to indicate that the box is now a placed box.
    map_data[box_square_nb.x][box_square_nb.y] = 4;

    // We blit the placed box image to the box position.
    SDL_BlitSurface(placed_box_surface, NULL, window, &box_position);
}

int have_all_boxes_been_placed(int number_of_boxes, int number_of_placed_boxes) {
    if (number_of_placed_boxes == number_of_boxes) {
        return 1;
    }

    return 0;
}


/*----------------------------------------
------------------------------------------
----------CLOSE THE GAME WINDOW-----------
------------------------------------------
----------------------------------------*/


void close_game_window(SDL_Surface* black_bar_vertical, SDL_Surface* exit_button,
                        SDL_Surface* blank_square, SDL_Surface* wall_square,
                        SDL_Surface* objective_square, SDL_Surface* box_square,
                        SDL_Surface* placed_box_surface, SDL_Surface* mario_surface,
                        SDL_Surface* window) {
    SDL_FreeSurface(black_bar_vertical);
    SDL_FreeSurface(exit_button);
    SDL_FreeSurface(blank_square);
    SDL_FreeSurface(wall_square);
    SDL_FreeSurface(objective_square);
    SDL_FreeSurface(box_square);
    SDL_FreeSurface(placed_box_surface);
    SDL_FreeSurface(mario_surface);
    SDL_FreeSurface(window);
}
