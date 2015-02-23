#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>

#include "window.h"
#include "map.h"
#include "status.h"

typedef struct {
    Map* map;
} Board;

Board* board_create(const char* map_file);
void board_destroy(Board* board);

Status board_move_mario(Board* board, DIRECTION direction);

Status board_move_sprite(Board* board, const unsigned int x,
                         const unsigned int y, const DIRECTION direction);
#endif  // BOARD_H_
