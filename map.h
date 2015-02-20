#ifndef MAP_H_
#define MAP_H_

#include "status.h"

enum {
    BLANK, WALL, BOX, OBJECTIVE, MARIO, BOX_OK, SPRITE_COUNT
} SPRITE;

typedef unsigned int SpriteId;

typedef struct {
    SpriteId sprite_id;
    WAYS direction;
} Square;

typedef struct {
    Square* square;
    unsigned int square_buffer_size;
    unsigned int square_count;
    unsigned int row;
    unsigned int column;
} Map;

Map* map_load(const char* filename);
Map* map_destroy(Map *map);

Square* map_get_square(const Map* map, unsigned int row,
                       unsigned int column, Status* status);
#endif  // MAP_H_
