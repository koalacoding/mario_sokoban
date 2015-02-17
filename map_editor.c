#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "map_editor.h"
#include "write_text_on_window.h"

// Avoiding some repetitions with this function.
void blit_surface(SDL_Surface* window, SDL_Surface* surface, SDL_Rect surface_position, int x,
                        int y) {
    surface_position.x = x;
    surface_position.y = y;
    SDL_BlitSurface(surface, NULL, window, &surface_position);
}

// Load and blit the sprites propositions of the right panel.
void load_and_blit_sprite_propositions(SDL_Surface* window, SDL_Surface* blank_square_black_border,
                                        SDL_Surface* *pointer_on_wall_square, SDL_Surface* *pointer_on_objective_square,
                                        SDL_Surface* *pointer_on_box_square) {
    SDL_Rect surface_position;

    blit_surface(window, blank_square_black_border, surface_position, 435, 150);

    *pointer_on_wall_square = IMG_Load("sprites/mur.jpg");
    blit_surface(window, *pointer_on_wall_square, surface_position, 435, 200);

    *pointer_on_objective_square = IMG_Load("sprites/objectif.png");
    blit_surface(window, *pointer_on_objective_square, surface_position, 435, 250);

    *pointer_on_box_square = IMG_Load("sprites/caisse.jpg");
    blit_surface(window, *pointer_on_box_square, surface_position, 435, 300);

}

// Function to change the shown selected sprite if the user clicks on a sprite.
void change_selected_sprite(SDL_Surface* window, SDL_Event event,
                            SDL_Surface* blank_square_black_border, SDL_Surface* wall_square,
                            SDL_Surface* objective_square, SDL_Surface* box_square) {
    SDL_Rect surface_position;

    if (event.button.x >= 435 && event.button.x <= 469) {
        // If the user clicks on the blank square sprite.
        if (event.button.y >= 150 && event.button.y <= 184) {
            // We change the selected square to the blank square sprite.
            blit_surface(window, blank_square_black_border, surface_position, 435, 40);
            SDL_Flip(window);
        }
        // If the user clicks on the wall sprite.
        else if (event.button.y >= 200 && event.button.y <= 234) {
            // We change the selected square to the wall sprite.
            blit_surface(window, wall_square, surface_position, 435, 40);
            SDL_Flip(window);
        }
        // If the user clicks on the objective sprite.
        else if (event.button.y >= 250 && event.button.y <= 284) {
            // We change the selected square to the wall sprite.
            blit_surface(window, objective_square, surface_position, 435, 40);
            SDL_Flip(window);
        }
        // If the user clicks on the box sprite.
        else if (event.button.y >= 300 && event.button.y <= 334) {
            // We change the selected square to the box sprite.
            blit_surface(window, box_square, surface_position, 435, 40);
            SDL_Flip(window);
        }
    }
}

void load_map_editor() {
    int window_height, window_width;

    SDL_Surface *window, *blank_square_black_border = NULL, *wall_square = NULL,
                *objective_square = NULL, *box_square = NULL;

    SDL_Rect surface_position;

    SDL_Event event;

    int x = 0, y = 0;

    int continue_loop = 1;

    window = SDL_SetVideoMode(500, 408, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    window_width = 408;
    window_height = 408;

    // We fill the window with a white background.
    SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));

    blank_square_black_border = IMG_Load("sprites/blank_black_border.jpg");

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
    blit_surface(window, blank_square_black_border, surface_position, 435, 40);

    // Sprites propositions.

    load_and_blit_sprite_propositions(window, blank_square_black_border, &wall_square,
                                        &objective_square, &box_square);
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

                continue_loop = 0;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) { // Mouse left click.
                    // If the user clicks on a sprite proposition, we change the selected sprite.
                    change_selected_sprite(window, event, blank_square_black_border, wall_square,
                    objective_square, box_square);
                }

                break;
        }
    }
}
