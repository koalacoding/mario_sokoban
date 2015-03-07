// IS THIS FILE IS NEEDED ?

#include "board.h"

#include "map.h"
#include "status.h"

Board* board_create(const char* map_file) {
    Board* new_board = NULL;
    Board* board = NULL;

    board = (Board*)malloc(sizeof(Board));
    if (board == NULL) {
        goto end;
    }

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


Status board_move_mario(Board* board, const DIRECTION direction) {
    Status status;
    status = board_move_sprite(board, board->map->mario.x, board->map->mario.y,
                               direction);
    return status;
}

Status board_move_sprite(Board* board, const unsigned int x,
                         const unsigned int y, const DIRECTION direction) {
    Status status;
    Square* square = NULL;
    Square* new_square = NULL;
    unsigned int new_x = x, new_y = y;

    switch(direction) {
        case DIRECTION_UP:
            new_y = y - 1;
            break;
        case DIRECTION_DOWN:
            new_y = y + 1;
            break;
        case DIRECTION_LEFT:
            new_x = x - 1;
            break;
        case DIRECTION_RIGHT:
            new_x = x + 1;
            break;
    }
    debug("moving sprite from %d,%d to %d,%d (direction %d)\n", x, y, new_x,
          new_y, direction);

    // get future square (check bounds...)
    new_square = map_get_square(board->map, new_x, new_y, &status);
    if (status.code != MARIO_STATUS_SUCCESS) {
       return status;
    }

    // get current square
    square = map_get_square(board->map, x, y, &status);
    if (status.code != MARIO_STATUS_SUCCESS) {
       return status;
    }

    // move a box
    if (square->square_id == SQUARE_BOX) {
        if (new_square->square_id != SQUARE_BLANK &&
            new_square->square_id != SQUARE_OBJECTIVE) {
            status.code = MARIO_STATUS_BLOCKED;
            status.message = "box is blocked";
            return status;
        }

        // make the move
        if (new_square->square_id == SQUARE_OBJECTIVE) {
            new_square->square_id = SQUARE_BOX_OK;
        } else {
            new_square->square_id = SQUARE_BOX;
        }
        square->square_id = SQUARE_BLANK;
        status.code = MARIO_STATUS_SUCCESS;
        return status;
    }

    // move mario
    if (square->square_id == SQUARE_MARIO) {
        if (new_square->square_id != SQUARE_BLANK &&
            new_square->square_id != SQUARE_BOX) {
            status.code = MARIO_STATUS_BLOCKED;
            status.message = "mario is blocked";
            return status;
        }

        // make the move
        if (new_square->square_id == SQUARE_BOX) {
            status = board_move_sprite(board, new_x, new_y, direction);
            if (status.code != MARIO_STATUS_SUCCESS) {
                return status;
            }
        }
        square->square_id = SQUARE_BLANK;
        new_square->square_id = SQUARE_MARIO;
        new_square->direction = direction;

        board->map->mario.x = new_x;
        board->map->mario.y = new_y;

        status.code = MARIO_STATUS_SUCCESS;
        return status;
    }

    status.code = MARIO_STATUS_ERROR;
    status.message = "this sprite as undefined move";
    return status;
}
