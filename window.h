#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

// this struct contains only one member but will probably grow
typedef struct {
    SDL_Surface* surface;
} Window;

Window* window_create(const char* caption, const unsigned int width,
                      const unsigned int height);

void window_destroy(Window* window);

#endif  // WINDOW_H_
