#include <SDL/SDL.h>

// Avoiding some repetitions with this function.
void blit_surface(SDL_Surface* window, SDL_Surface* surface, int x, int y) {

    SDL_Rect surface_position;

    surface_position.x = x;
    surface_position.y = y;
    SDL_BlitSurface(surface, NULL, window, &surface_position);
}
