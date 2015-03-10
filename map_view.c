#include "map_view.h"

#include "game.h"
#include "sprite.h"
#include "map.h"
#include "window.h"

static void destroy_sprites(MapView* map_view);
static Status load_sprites(MapView* map_view);

// TODO
static SDL_Surface* get_sprite_surface(const MapView* map_view,
                                       const Square* square);

MapView* map_view_create(Map* map) {
    MapView* new_view = NULL;
    Status status = { MARIO_STATUS_ERROR, "error unknown" };

    MapView* view = malloc(sizeof(MapView));
    if (view == NULL) {
        fprintf(stderr, "cannot allocate MapView\n");
        goto end;
    }
    memset(view, 0, sizeof(MapView));

    status = load_sprites(view);
    if (status.code != MARIO_STATUS_SUCCESS) {
        fprintf(stderr, "cannot create MapView: %s\n", status.message);
        goto end;
    }
    view->map = map;
    view->square_width = view->sprites[SQUARE_BLANK]->image[0]->w;
    view->square_height = view->sprites[SQUARE_BLANK]->image[0]->h;

    // no error, new_view can be defined and returned
    new_view = view;

end:
    // only cleanup if anything went wrong
    if (new_view == NULL && view != NULL) {
        if (view->sprites) {
            destroy_sprites(view);
        }
        free(view);
    }
    return new_view;
}

void map_view_destroy(MapView* map_view) {
    if (map_view->sprites) {
        destroy_sprites(map_view);
    }
    free(map_view);
}

// example with Status code !!! non-academic, free-style ;) !!!
// static keyword means (ONLY FOR FUNCTIONS) that it won't be available outside
// this source file
static Status load_sprites(MapView* map_view) {
    Status status = { MARIO_STATUS_ERROR, "cannot load sprites" };
    unsigned int i = 0;

    // cleanup current sprites if any
    if (map_view->sprites) {
        destroy_sprites(map_view);
    }

    // load sprites (these should be referenced by the map and not hardcoded)
    map_view->sprites = (Sprite**)malloc(sizeof(Sprite*)*SQUARE_ID_COUNT);
    if (map_view->sprites == NULL) {
        status.message = "cannot load sprites, memory allocation failed";
        goto end;
    }

    map_view->sprites[SQUARE_BLANK] = sprite_create("sprites/blank.jpg");
    map_view->sprites[SQUARE_WALL] = sprite_create("sprites/wall.jpg");
    map_view->sprites[SQUARE_BOX] = sprite_create("sprites/box.jpg");
    map_view->sprites[SQUARE_OBJECTIVE] =
            sprite_create("sprites/objective.png");
    map_view->sprites[SQUARE_MARIO] = sprite_create_faced(
                                                 "sprites/mario_up.gif",
                                                 "sprites/mario_down.gif",
                                                 "sprites/mario_left.gif",
                                                 "sprites/mario_right.gif");
    map_view->sprites[SQUARE_BOX_OK] = sprite_create("sprites/box_ok.jpg");
    map_view->sprite_count = SQUARE_ID_COUNT;

    // cancel everything if any sprite is missing
    for (i = 0; i < map_view->sprite_count; i++) {
        if (map_view->sprites[i] == NULL) {
            fprintf(stderr, "missing sprite %d\n", i);
            status.message = "cannot load sprites, missing sprite(s)";
            goto end;
        }
    }

    status.code = MARIO_STATUS_SUCCESS;

end:
    if (status.code != MARIO_STATUS_SUCCESS) {
        if (map_view->sprites) {
            destroy_sprites(map_view);
        }
    }
    return status;
}

static void destroy_sprites(MapView* map_view) {
    unsigned int i = 0;
    for (i = 0; i < map_view->sprite_count; i++) {
        if (map_view->sprites[i] == NULL)
            continue;
        sprite_destroy(map_view->sprites[i]);
        map_view->sprites[i] = NULL;
    }
    free(map_view->sprites);
    map_view->sprites = NULL;
    map_view->sprite_count = 0;
}

static SDL_Surface* get_sprite_surface(const MapView* map_view,
                                       const Square* square) {
    const Sprite* sprite = map_view->sprites[square->square_id];
    return sprite->image[square->direction];
}

// FIXME: destrect's x,y specify origin, w and h members should be used aswell
Status map_view_draw(MapView* map_view, SDL_Surface* surface,
                     SDL_Rect* destrect) {
    Status status = { MARIO_STATUS_ERROR, "cannot draw map" };
    int x_offset = 0;
    int y_offset = 0;
    int ret = -1;

    if (destrect) {
        x_offset = destrect->x;
        y_offset = destrect->y;
    }

    // WARNING: destrect's width/height should be well set
    // fill the window with white
    ret = SDL_FillRect(surface, NULL /*destrect*/,
                       SDL_MapRGB(surface->format, 255, 255, 255));
    if (ret != 0) {
        status.message = SDL_GetError();
        goto end;
    }

    // draw every squares
    // TODO: a function for getting rect and surface
    unsigned int row = 0;
    for (row = 0; row < map_view->map->row; row++) {
        unsigned int column = 0;
        for (column = 0; column < map_view->map->column; column++) {
            Status status;
            SDL_Rect rect;
            const Square* square = map_get_square(map_view->map, column, row,
                                                  &status);
            if (square == NULL) {
                goto end;
            }
            rect.x = map_view->square_width * column + x_offset;
            rect.y = map_view->square_height * row + y_offset;
            ret = SDL_BlitSurface(get_sprite_surface(map_view, square), NULL,
                                  surface, &rect);
            if (ret != 0) {
                status.message = SDL_GetError();
                goto end;
            }
        }
    }
    status.code = MARIO_STATUS_SUCCESS;

    SDL_Flip(surface);

end:
    return status;
}

static void map_view_event_handler(Game* game, const SDL_Event* event,
                                   void* param) {
    //debug("menu_event_handler\n");
    bool draw_map = true;
    SDL_Surface* surface = game->window->surface;
    MapView* view = (MapView*)param;
    switch(event->type) {
        case SDL_KEYDOWN:
            debug("map view got %d\n", event->key.keysym.sym);
            switch(event->key.keysym.sym) {
                case SDLK_UP:
                    map_move_mario(view->map, DIRECTION_UP);
                break;
                case SDLK_DOWN:
                    map_move_mario(view->map, DIRECTION_DOWN);
                break;
                case SDLK_LEFT:
                    map_move_mario(view->map, DIRECTION_LEFT);
                break;
                case SDLK_RIGHT:
                    map_move_mario(view->map, DIRECTION_RIGHT);
                break;
            default:
                draw_map = false;
            }
            if (draw_map) {
                map_view_draw(view, surface, NULL);
            }
        break;
    }
}

void map_view_get_event_handler(MapView* map_view,
                                struct EventHandler* handler) {
    handler->function = map_view_event_handler;
    handler->param = map_view;
}
