#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>

#include "sprite.h"
#include "map.h"

typedef struct {
    Sprite* sprite;
    // we could use directly SDL_rect to cache position but has performance
    // is not an issue I prefer abstraction and avoiding having SDL include here
    uint16_t row;
    uint16_t column;
} Square;

typedef struct {
    Sprite** sprites;
    Square** squares;
    Map* map;
    //Window window;
} Board;

Board* board_create(const char* map_file);
void board_destroy(Board* game);
void board_show(Board* game);

#endif  // BOARD_H_
