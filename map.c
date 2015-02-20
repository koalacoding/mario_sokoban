// THIS FILE HEADER
#include "map.h"

// STANDARD HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// EXTERNAL HEADERS
// ...

// CUSTOM HEADERS
#include "status.h"


// this is the max number of column supported, this value will help when parsing
// the map file to determine line buffer size
#define MAP_COLUMN_MAX 64

// it is overkill for what the exercise is asking, row and column are not
// hardcoded (should be 12x12)
Map* map_load(const char* filename) {
    FILE* fp;
    Map* map = NULL;
    Map* new_map = NULL;

    map = (Map*)malloc(sizeof(Map));
    memset(map, 0, sizeof(Map));

    // create a buffer too small, forcing realloc to be executed for the example
    // the buffer will hold a table of int representing the id of sprites
    map->square_buffer_size = 4*sizeof(Square);
    map->square = (Square*)malloc(map->square_buffer_size);
    memset(map->square, 0, map->square_buffer_size);

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        // if fopen fails, errno is set, perror format a message according to
        // errno value, see http://www.cplusplus.com/reference/cstdio/perror/
        perror("Cannot open map %s\n");
        goto end;
    }

    // parse line by line the map file
    while (1) {
        // a line is composed of an int (the sprite id) and a space as separator
        // ex: 1234 567 ...
        const unsigned int line_size = MAP_COLUMN_MAX*(sizeof(SpriteId)+1);
        char* token = NULL;
        char line[line_size];

        // read a line to be parsed
        memset(line, 0, line_size);
        if (fgets(line, line_size, fp) == NULL) {
            break;
        }

        // extract integers and get column count with space as separator
        token = strtok(line, " ");
        while (token) {
            // make sure buffer is large enough
            // TODO: this could be struct/function
            if (map->square_count*sizeof(Square) >=
                    map->square_buffer_size) {
                // add only 8 items so there will be more realloc (example)
                const unsigned int new_buffer_size =
                        map->square_buffer_size + 8*sizeof(Square);

                Square* new_alloc = realloc(map->square, new_buffer_size);
                if (new_alloc == NULL) {
                    fprintf(stderr, "allocation failed (%d bytes)\n",
                            new_buffer_size);
                    goto end;
                }
                debug("%s:%d realloc %d bytes (%d cells)\n", __FILE__, __LINE__,
                      new_buffer_size, new_buffer_size/sizeof(Square));
                map->square = new_alloc;
                map->square_buffer_size = new_buffer_size;
            }
            sscanf(token, "%d", &map->square[map->square_count].sprite_id);
            map->square[map->square_count].direction = DIRECTION_DOWN;
            map->square_count++;

            token = strtok(NULL, " ");
        }
        map->row++;

        // number of column must be constant
        if (map->row == 1) {
            map->column = map->square_count;
        } else {
            if (map->column * map->row != map->square_count) {
                fprintf(stderr, "wrong number of columns, line %d\n", map->row);
                goto end;
            }
        }
    }
    new_map = map;
end:
    if (new_map == NULL) {
        map_destroy(map);
    }
    if (fp != NULL) {
        fclose(fp);
    }
    return new_map;
}

Map* map_destroy(Map* map) {
    if (map->square != NULL) {
        free(map->square);
        map->square = NULL;  // ensure it will break on reuse
    }
    free(map);
}

// this function return error by setting status structure, no need for the
// 'goto' statement here as there's nothing to cleanup
Square* map_get_square(const Map* map, unsigned int row,
                       unsigned int column, Status* status) {
    status->code = MARIO_STATUS_ERROR;

    // sometimes it's good to check input, this is called 'guard statement'
    if (column > map->column || row > map->row) {
        status->message = "no square at those (row,column)";
        return NULL;
    }
    status->code = MARIO_STATUS_SUCCESS;
    return &map->square[row*map->column + column];
}
