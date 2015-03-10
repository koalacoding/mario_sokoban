#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

typedef struct Window { SDL_Surface* surface; } Window;

Window* window_create(const char* caption, const unsigned int width,
                      const unsigned int height);

void window_destroy(Window* window);

#endif  // WINDOW_H_
