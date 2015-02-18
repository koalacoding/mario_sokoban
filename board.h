#ifndef BOARD_H_
#define BOARD_H_

#include <stdint.h>

#include "window.h"
#include "map.h"

typedef struct {
    Map* map;
} Board;

Board* board_create(const char* map_file);
void board_destroy(Board* board);

#endif  // BOARD_H_
