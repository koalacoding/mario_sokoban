#ifndef BLIT_SURFACE_H_INCLUDED
#define BLIT_SURFACE_H_INCLUDED

// Make the blitting of surfaces easier, with less repetitions.
void blit_surface(SDL_Surface* window, SDL_Surface* surface, int x, int y);

#endif
