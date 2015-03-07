#include <SDL/SDL.h>

#include "../../structures/structures.h"

int has_surface_been_clicked(int mouse_pointer_x, int mouse_pointer_y, SDL_Rect surface_position, SDL_Surface* surface) {
    if ((mouse_pointer_x >= surface_position.x && mouse_pointer_y >= surface_position.y)
        && (mouse_pointer_x <= surface_position.x + surface->w
            && mouse_pointer_y <= surface_position.y + surface->h)) {
        return 1;
    }

    return 0;
}
