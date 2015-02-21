#include "window.h"

static void destroy_sprites(Window* window);
static SDL_Surface* get_sprite_surface(const Window* window,
                                       const Square* square);

Window* window_create(const char* caption, const unsigned int width,
                      const unsigned int height) {
    Window* new_window = NULL;
    int ret = -1;

    Window* window = malloc(sizeof(Window));
    if (window == NULL) {
        fprintf(stderr, "Cannot allocate window %s\n", caption);
        goto end;
    }

    window->sprites = NULL;
    window->sprite_count = 0;

    // CAREFULL! The surface returned is freed by SDL_Quit() and should'nt be
    // freed by the caller
    window->surface = SDL_SetVideoMode(width, height, 32,
                                       SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (window->surface == NULL) {
        fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
        goto end;
    }

    SDL_WM_SetCaption(caption, NULL);

    // fill the window with white
    ret = SDL_FillRect(window->surface, NULL,
                       SDL_MapRGB(window->surface->format, 255, 255, 255));
    if (ret != 0) {
        fprintf(stderr, "SDL_FillRect failed on %s\n", SDL_GetError());
        goto end;
    }

    // as the window as SDL_DOUBLEBUF flag, it should be flipped each time
    // the changes have to be draw (it flip the buffers)
    ret = SDL_Flip(window->surface);
    if (ret != 0) {
        fprintf(stderr, "Failed to swap the buffers: %s\n", SDL_GetError());
        goto end;
    }

    // no error, new_window can be defined and returned
    new_window = window;

end:
    // only cleanup if anything went wrong
    if (new_window == NULL) {
        if (window != NULL) {
            free(window);
        }
    }
    return new_window;
}

void window_destroy(Window* window) {
    if (window->sprites) {
        destroy_sprites(window);
    }
    //SDL_FreeSurface(window->surface);  // DON'T !!! See SDL_SetVideoMode()
    free(window);
}

// example with Status code !!! non-academic, free-style ;) !!!
// static keyword means HERE that the function won't be available outside
// this source file (it's only available for the current translation unit)
static Status load_sprites(Window* window, Map* map) {
    Status status = { MARIO_STATUS_ERROR, "cannot load sprites" };
    unsigned int i = 0;

    // cleanup current sprites if any
    if (window->sprites) {
        destroy_sprites(window);
    }

    // load sprites (these should be referenced by the map and not hardcoded)
    window->sprites = (Sprite**)malloc(sizeof(Sprite*)*SPRITE_ID_COUNT);
    if (window->sprites == NULL) {
        status.message = "cannot load sprites, memory allocation failed";
        goto end;
    }
    window->sprite_count = SPRITE_ID_COUNT;

    window->sprites[BLANK] = sprite_create("sprites/blank.jpg");
    window->sprites[WALL] = sprite_create("sprites/wall.jpg");
    window->sprites[BOX] = sprite_create("sprites/box.jpg");
    window->sprites[OBJECTIVE] = sprite_create("sprites/objective.png");
    window->sprites[MARIO] = sprite_create_faced("sprites/mario_up.gif",
                                                 "sprites/mario_down.gif",
                                                 "sprites/mario_left.gif",
                                                 "sprites/mario_right.gif");
    window->sprites[BOX_OK] = sprite_create("sprites/box_ok.png");

    // cancel everything if any sprite is missing
    for (i = 0; i < window->sprite_count; i++) {
        if (window->sprites[i] == NULL) {
            fprintf(stderr, "missing sprite\n");
            status.message = "cannot load sprites, missing sprite(s)";
            goto end;
        }
    }

    status.code = MARIO_STATUS_SUCCESS;

end:
    if (status.code != MARIO_STATUS_SUCCESS) {
        if (window->sprites) {
            destroy_sprites(window);
        }
    }
    return status;
}

static void destroy_sprites(Window* window) {
    unsigned int i = 0;
    for (i = 0; i < window->sprite_count; i++) {
        if (window->sprites[i] == NULL)
            continue;
        sprite_destroy(window->sprites[i]);
        window->sprites[i] = NULL;
    }
    free(window->sprites);
    window->sprites = NULL;
    window->sprite_count = 0;
}

static SDL_Surface* get_sprite_surface(const Window *window,
                                       const Square *square) {
    const Sprite* sprite = window->sprites[square->sprite_id];
    return sprite->image[square->direction];
}

Status window_display_map(Window* window, Map* map) {
    Status status = { MARIO_STATUS_ERROR, "cannot load map" };
    int ret = -1;
    unsigned int square_width = 0;
    unsigned int square_height = 0;

    status = load_sprites(window, map);
    if (status.code != MARIO_STATUS_SUCCESS) {
        // don't set status.message, as load_sprites is already returning a
        // status, message will be set to exact error status
        goto end;
    }

    // TODO: remove ugly code ?
    square_width = window->sprites[BLANK]->image[0]->w;
    square_height = window->sprites[BLANK]->image[0]->h;

    // resize the window according to the map size
    window->surface = SDL_SetVideoMode(square_width*map->column,
                                       square_height*map->row,
                                       32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    if (window->surface == NULL) {
        status.message = SDL_GetError();
        goto end;
    }

    // fill the window with white
    ret = SDL_FillRect(window->surface, NULL,
                       SDL_MapRGB(window->surface->format, 255, 255, 255));
    if (ret != 0) {
        status.message = SDL_GetError();
        goto end;
    }

    // draw every squares
    // TODO: a function for getting rect and surface
    unsigned int row = 0;
    for (row = 0; row < map->row; row++) {
        unsigned int column = 0;
        for (column = 0; column < map->column; column++) {
            Status status;
            SDL_Rect rect;
            const Square* square = map_get_square(map, column, row, &status);
            if (square == NULL) {
                goto end;
            }
            rect.x = window->surface->w / map->column * column;
            rect.y = window->surface->h / map->row * row;
            ret = SDL_BlitSurface(get_sprite_surface(window, square),
                                  NULL, window->surface, &rect);
            if (ret != 0) {
                status.message = SDL_GetError();
                goto end;
            }
        }
    }

    // as the window as SDL_DOUBLEBUF flag, it should be flipped each time
    // the changes have to be draw (it flip the buffers)
    ret = SDL_Flip(window->surface);
    if (ret != 0) {
        status.message = SDL_GetError();
        goto end;
    }

    status.code = MARIO_STATUS_SUCCESS;

end:
    if (status.code != MARIO_STATUS_SUCCESS) {
        fprintf(stderr, "failed to display map: %s\n", status.message);
        // could be omitted as it will be destroyed with the window
        if (window->sprites) {
            destroy_sprites(window);
        }
    }
    return status;
}
