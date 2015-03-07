#ifndef MAP_H_
#define MAP_H_

#include "status.h"

typedef enum {
    SPRITE_BLANK,
    SPRITE_WALL,
    SPRITE_BOX,
    SPRITE_OBJECTIVE,
    SPRITE_MARIO,
    SPRITE_BOX_OK,
    SPRITE_ID_COUNT
} SPRITE_ID;

typedef enum {
    DIRECTION_UP,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_COUNT
} DIRECTION;

typedef unsigned int SpriteId;

typedef struct {
    SpriteId sprite_id;
    DIRECTION direction;
} Square;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position mario;
    Square* square;
    unsigned int square_buffer_size;
    unsigned int square_count;
    unsigned int row;
    unsigned int column;
    unsigned int objective_count;
} Map;

Map* map_load(const char* filename);
Map* map_destroy(Map *map);

Square* map_get_square(const Map* map, const unsigned int x,
                       const unsigned int y, Status* status);

Square* map_set_square(Map* map, const unsigned int x, const unsigned int y,
                       const SpriteId sprite_id, const DIRECTION direction,
                       Status* status);
#endif  // MAP_H_
