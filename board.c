#include "board.h"

#include "map.h"
#include "sprite.h"

Board* board_create(const char* map_file) {
    Board* new_board = NULL;
    Board* board = NULL;
    Map* map = NULL;
    unsigned int i = 0;

    board = (Board*)malloc(sizeof(Board));
    if (board == NULL) {
        goto end;
    }

    // load map
    board->map = map_load(map_file);
    if (board->map == NULL) {
        goto end;
    }

    board->sprites = (Sprite**)malloc(sizeof(Sprite*)*SPRITE_COUNT);

    // load sprites (these should be referenced by the map and not hardcoded)
    board->sprites[BLANK] = sprite_create("sprites/blank.jpg");
    board->sprites[WALL] = sprite_create("sprites/wall.jpg");
    board->sprites[BOX] = sprite_create("sprites/box.jpg");
    board->sprites[OBJECTIVE] = sprite_create("sprites/objective.png");
    board->sprites[MARIO] = sprite_create_faced("sprites/mario_up.gif",
                                                "sprites/mario_down.gif",
                                                "sprites/mario_left.gif",
                                                "sprites/mario_right.gif");
    board->sprites[BOX_OK] = sprite_create("sprites/box_ok.png");

    // cancel everything if any sprite is missing
    for (i = 0; i < SPRITE_COUNT; i++) {
        if (board->sprites[i] == NULL) {
            fprintf(stderr, "map loading cancelled (a sprite is missing)\n");
            goto end;
        }
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
    if (board->sprites) {
        for (i = 0; i < SPRITE_COUNT; i++) {
            Sprite* sprite = board->sprites[i];
            if (sprite) {
                sprite_destroy(sprite);
            }
        }
        free(board->sprites);
    }
    if (board->map) {
        map_destroy(board->map);
    }
    free(board);
}

void board_show(Board* board) {
    unsigned int row = 0;
//    for (row = 0; row < board->map->row; row++) {
//        unsigned int column = 0;
//        for (column = 0; column < board->map->column; column++) {
//            unsigned int id = board->map->squares[column][row];
//            SDL_BlitSurface(board->sprites[id], NULL)
//        }
//    }
}
