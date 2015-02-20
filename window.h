#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

#include "status.h"
#include "map.h"
#include "sprite.h"

typedef struct {
    SDL_Surface* surface;
    Sprite** sprites;  // <=> Sprite* sprites[]
    unsigned int sprite_count;
} Window;

Window* window_create(const char* caption, const unsigned int width,
                      const unsigned int height);

void window_destroy(Window* window);

Status window_display_map(Window* window, Map* map);


#endif  // WINDOW_H_
