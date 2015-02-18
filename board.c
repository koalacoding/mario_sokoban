#include "board.h"

#include "map.h"

Board* board_create(const char* map_file) {
    Board* new_board = NULL;
    Board* board = NULL;

    board = (Board*)malloc(sizeof(Board));
    if (board == NULL) {
        goto end;
    }

    // load map
    board->map = map_load(map_file);
    if (board->map == NULL) {
        goto end;
    }

    new_board = board;

end:
    if (new_board == NULL) {
        board_destroy(board);
    }
    return new_board;
}

void board_destroy(Board* board) {
    unsigned int i = 0;
    if (board == NULL) {
        return;
    }
    if (board->map) {
        map_destroy(board->map);
    }
    free(board);
}
