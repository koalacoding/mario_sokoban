#ifndef MAP_H_
#define MAP_H_

#include "status.h"

typedef enum {
    SQUARE_BLANK,
    SQUARE_WALL,
    SQUARE_BOX,
    SQUARE_OBJECTIVE,
    SQUARE_MARIO,
    SQUARE_BOX_OK,
    SQUARE_ID_COUNT
} SQUARE_ID;

typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_COUNT
} DIRECTION;

typedef struct Square {
    SQUARE_ID square_id;
    DIRECTION direction;
} Square;

typedef struct Position {
    int x;
    int y;
} Position;

typedef struct Map {
    Position mario;
    Square* square;
    unsigned int square_buffer_size;
    unsigned int square_count;
    unsigned int row;
    unsigned int column;
    unsigned int objective_count;
} Map;

Map* map_create(const char* filename);
Map* map_create_empty(const unsigned int column, const unsigned int row);
Map* map_destroy(Map *map);

Square* map_get_square(const Map* map, const unsigned int x,
                       const unsigned int y, Status* status);

Square* map_set_square(Map* map, const unsigned int x, const unsigned int y,
                       const SQUARE_ID square_id, const DIRECTION direction,
                       Status* status);

Status map_move_mario(Map* map, DIRECTION direction);

Status map_move_sprite(Map* map, const unsigned int x, const unsigned int y,
                       const DIRECTION direction);

#endif  // MAP_H_
