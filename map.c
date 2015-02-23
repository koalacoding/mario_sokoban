// THIS FILE HEADER
#include "map.h"

// STANDARD HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// EXTERNAL HEADERS
// ...

// CUSTOM HEADERS
#include "status.h"


// this is the max number of column supported, this value will help when parsing
// the map file to determine line buffer size
#define MAP_COLUMN_MAX 64

// it is overkill for what the exercise is asking, row and column are not
// hardcoded (should be 12x12)
static Map* parse_map_file(const char* filename) {
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
            map->square_count++;
            token = strtok(NULL, " ");
        }
        map->row++;

        // sanity check: number of column must be constant
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

// load a map from a file (parse it then initialize some usefull map members)
//
// this function won't use any goto style cleanup code, I find the following
// boring (but more academic "never use goto..." :) )
Map* map_load(const char* filename) {
    unsigned int x, y;
    Map* map = NULL;
    bool found_mario = false;

    map = parse_map_file(filename);
    if (map == NULL) {
        return NULL;
    }

    // browse the parsed data to initialize more members
    // those should be read from the map
    for (y = 0; y < map->row; y++) {
        for (x = 0; x < map->column; x++) {
            Status status;
            Square* square = map_get_square(map, x, y, &status);
            if (status.code != MARIO_STATUS_SUCCESS) {
                fprintf(stderr, "error: %s\n", status.message);
                map_destroy(map);
                return NULL;
            }

            // default direction is up
            square->direction = DIRECTION_UP;

            switch(square->sprite_id) {
                case SPRITE_OBJECTIVE:
                    map->objective_count++;
                    break;
                case SPRITE_MARIO:
                    found_mario = true;
                    map->mario.x = x;
                    map->mario.y = y;
                    break;
            }
        }
    }

    // fix missing mario position on the map by giving a default one
    if (found_mario == false) {
        Status status;
        map->mario.x = 5;
        map->mario.y = 3;
        map_set_square(map, map->mario.x, map->mario.y, SPRITE_MARIO,
                       DIRECTION_DOWN, &status);
        if (status.code != MARIO_STATUS_SUCCESS) {
            fprintf(stderr, "error: %s\n", status.message);
            map_destroy(map);
            return NULL;
        }
    }
    return map;
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
Square* map_get_square(const Map* map, const unsigned int x,
                       const unsigned int y, Status* status) {
    status->code = MARIO_STATUS_ERROR;

    // sometimes it's good to check input, this is called 'guard statement'
    if (x >= map->column || y >= map->row) {
        status->message = "invalid arguments, square is out of bounds";
        return NULL;
    }
    status->code = MARIO_STATUS_SUCCESS;
    return &map->square[map->column * y + x];
}

Square* map_set_square(Map* map, const unsigned int x, const unsigned int y,
                       const SpriteId sprite_id, const DIRECTION direction,
                       Status* status) {
    Square* square = NULL;
    status->code = MARIO_STATUS_ERROR;

    square = map_get_square(map, x, y, status);
    if (status->code != MARIO_STATUS_SUCCESS) {
        return NULL;
    }

    square->sprite_id = sprite_id;
    square->direction = direction;
    status->code = MARIO_STATUS_SUCCESS;
    return square;
}