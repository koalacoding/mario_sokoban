#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    map->sprite_id_buffer_size = 4*sizeof(SpriteId);
    map->sprite_id = (SpriteId*)malloc(map->sprite_id_buffer_size);
    memset(map->sprite_id, 0, map->sprite_id_buffer_size);

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
            if (map->sprite_id_count*sizeof(SpriteId) >=
                    map->sprite_id_buffer_size) {
                // add only 8 items so there will be more realloc (example)
                const unsigned int new_buffer_size =
                        map->sprite_id_buffer_size + 8*sizeof(SpriteId);

                int* new_alloc = realloc(map->sprite_id, new_buffer_size);
                if (new_alloc == NULL) {
                    fprintf(stderr, "allocation failed (%d bytes)\n",
                            new_buffer_size);
                    goto end;
                }
                debug("%s:%d realloc %d bytes (%d cells)\n", __FILE__, __LINE__,
                      new_buffer_size, new_buffer_size/sizeof(SpriteId));
                map->sprite_id = new_alloc;
                map->sprite_id_buffer_size = new_buffer_size;
            }
            sscanf(token, "%d", &map->sprite_id[map->sprite_id_count++]);
            token = strtok(NULL, " ");
        }
        map->row++;

        // number of column must be constant
        if (map->row == 1) {
            map->column = map->sprite_id_count;
        } else {
            if (map->column * map->row != map->sprite_id_count) {
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
    if (map->sprite_id != NULL) {
        free(map->sprite_id);
        map->sprite_id = NULL;  // ensure it will break on reuse
    }
    free(map);
}
