#ifndef MAP_H_
#define MAP_H_

typedef struct {
    unsigned int* squares;
    unsigned int squares_count;
    unsigned int row;
    unsigned int column;
} Map;

Map* map_load(const char* filename);
Map* map_destroy(Map *map);

#endif  // MAP_H_
