#include "map.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

#define MAP_COLUMN_MAX 12

// it is overkill for what the exercise is asking, row and column are not
// hardcoded (should be 12x12)
Map* map_load(const char* filename) {
    FILE* fp;
    unsigned int i = 0;
    const unsigned int default_square_count = 16;
    Map* map = NULL;
    Map* new_map = NULL;

    map = (Map*)malloc(sizeof(Map));
    memset(map, 0, sizeof(Map));

    map->squares = (int*)malloc(default_square_count * sizeof(int));
    map->squares_count = default_square_count;
    memset(map->squares, 0, sizeof(int) * map->squares_count);

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        // if fopen fails, errno is set and perror format a message from errno
        // see http://www.cplusplus.com/reference/cstdio/perror/
        perror("Cannot open map %s\n");
        goto end;
    }

    while (1) {
        char* line_offset = NULL;
        unsigned int current_index = 0;
        const unsigned int max_line = MAP_COLUMN_MAX*(sizeof(int)+1);
        char line[max_line];
        memset(line, 0, max_line);

        // read a line to be parsed
        if (fgets(line, max_line, fp) == NULL) {
            break;  // no more line to process
        }
        map->row++;

        // extract integers and get column count
        current_index = i;
        line_offset = strtok(line, " ");
        while (line_offset) {
            // make buffer large enough
            if (i >= map->squares_count) {
                const unsigned int added_entry_count = 2;
                debug("realloc: %d\n", (map->squares_count + added_entry_count)*sizeof(int));
                int* new_alloc = realloc(map->squares,
                    (map->squares_count + added_entry_count)*sizeof(int));
                if (new_alloc == NULL) {
                    fprintf(stderr, "allocation failed\n");
                    goto end;
                }
                map->squares = new_alloc;
                map->squares_count += added_entry_count;
            }
            sscanf(line_offset, "%d", &map->squares[i++]);
            line_offset = strtok(NULL, " ");
        }
        if (map->column == 0) {
            map->column = i - current_index;
        } else {
            // number of column must always be the same
            if (map->column != i - current_index) {
                fprintf(stderr, "invalid map file format");
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
    if (map->squares != NULL) {
        free(map->squares);
    }
    free(map);
}
