#ifndef MAP_H_
#define MAP_H_

#include "status.h"

enum {
    BLANK, WALL, BOX, OBJECTIVE, MARIO, BOX_OK, SPRITE_COUNT
} SPRITE;

typedef unsigned int SpriteId;

typedef struct {
    SpriteId* sprite_id;
    unsigned int sprite_id_buffer_size;
    unsigned int sprite_id_count;  // number of loaded sprites
    unsigned int row;
    unsigned int column;
} Map;

Map* map_load(const char* filename);
Map* map_destroy(Map *map);

SpriteId map_get_sprite_id(const Map* map, unsigned int row,
                           unsigned int column, Status* status);
#endif  // MAP_H_
