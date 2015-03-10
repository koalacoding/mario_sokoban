// see http://fr.wikipedia.org/wiki/Include_guard about this include guard
#ifndef SPRITE_H_
#define SPRITE_H_

#include <stdbool.h>  // only available in C99, this defines 'bool'

#include <SDL/SDL_image.h>

#include "map.h"

typedef struct Sprite {
  SDL_Surface* image[DIRECTION_COUNT];
  bool allocated_image[DIRECTION_COUNT];  // image must be free if 'true'
} Sprite;

Sprite* sprite_create(const char* image);

Sprite* sprite_create_faced(const char* image_up, const char* image_down,
                            const char* image_left, const char* image_right);

void sprite_destroy(Sprite* sprite);

#endif  // SPRITE_H_
