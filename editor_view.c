#include "editor_view.h"

#include <SDL_gfxPrimitives.h>

#include "game.h"
#include "window.h"
#include "sprite.h"
#include "map.h"
#include "map_view.h"

typedef struct Toolbar {
  SDL_Rect rect;  // position in the 'parent' surface
  struct ToolbarButton* button;
  unsigned int button_count;
  unsigned int button_width;
  unsigned int button_height;
  unsigned int selected_button;
} Toolbar;

typedef struct ToolbarButton {
  SDL_Rect rect;  // position in the toolbar's 'parent' surface
  SDL_Surface* image;
} ToolbarButton;

typedef enum { DRAW_IN, DRAW_OUT } DRAW_SIDE;

EditorView* editor_view_create() {
  EditorView* editor = NULL;
  EditorView* new_editor = NULL;
  int i = 0;

  editor = malloc(sizeof(EditorView));
  if (editor == NULL) {
    fprintf(stderr, "cannot allocate EditorView\n");
    goto end;
  }

  editor->map = map_create_empty(12, 12);
  if (editor->map == NULL) {
    goto end;
  }

  editor->map_view = map_view_create(editor->map);
  if (editor->map_view == NULL) {
    goto end;
  }

  editor->toolbar = malloc(sizeof(Toolbar));
  if (editor->toolbar == NULL) {
    goto end;
  }
  editor->toolbar->button_count = SQUARE_ID_COUNT;
  editor->toolbar->button = malloc(sizeof(ToolbarButton) * SQUARE_ID_COUNT);
  if (editor->toolbar->button == NULL) {
    goto end;
  }

  for (i = 0; i < editor->toolbar->button_count; i++) {
    // TODO: ask the sprites to a sprite factory, don't use map_view
    const unsigned int square_width = editor->map_view->square_width;
    const unsigned int square_height = editor->map_view->square_height;
    editor->toolbar->button[i].image =
        editor->map_view->sprites[i]->image[DIRECTION_DOWN];
    editor->toolbar->rect.x = map_view_get_width(editor->map_view);
    editor->toolbar->rect.y = 10;
    editor->toolbar->rect.w = 55;
    editor->toolbar->rect.h = map_view_get_height(editor->map_view);
    editor->toolbar->button[i].rect.x = 10 + editor->toolbar->rect.x;
    editor->toolbar->button[i].rect.y =
        (10 + square_height) * i + editor->toolbar->rect.y;
    editor->toolbar->button[i].rect.w = square_width;
    editor->toolbar->button[i].rect.h = square_height;
  }
  editor->toolbar->selected_button = SQUARE_BLANK;

  new_editor = editor;
end:
  if (new_editor == NULL) {
    editor_view_destroy(editor);
  }
  return new_editor;
}

void editor_view_destroy(EditorView* editor_view) {
  int i = 0;
  if (editor_view->toolbar) {
    if (editor_view->toolbar->button) {
      free(editor_view->toolbar->button);
      editor_view->toolbar->button = NULL;
    }
    free(editor_view->toolbar);
    editor_view->toolbar = NULL;
  }
  if (editor_view->map_view) {
    map_view_destroy(editor_view->map_view);
    editor_view->map_view = NULL;
  }
  if (editor_view->map) {
    map_destroy(editor_view->map);
    editor_view->map = NULL;
  }
  free(editor_view);
}

unsigned int editor_view_get_width(EditorView* editor_view) {
  return map_view_get_width(editor_view->map_view) +
         editor_view->toolbar->rect.w;
}

unsigned int editor_view_get_height(EditorView* editor_view) {
  return map_view_get_height(editor_view->map_view);
}

static draw_rectangle(SDL_Surface* surface, Sint16 x1, Sint16 y1, Sint16 x2,
                      Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                      const Sint16 thickness, const DRAW_SIDE draw_side) {
  int i = 0;
  Sint16 side = 1;
  if (draw_side == DRAW_IN) {
    side = -1;
  }

  for (i = 0; i < thickness; i++) {
    Sint16 k = side * i;
    rectangleRGBA(surface, x1 - k, y1 - k, x2 + k, y2 + k, r, g, b, a);
  }
}

// TODO: think about surface argument, maybe it should keep as member
static void draw_toolbar(EditorView* editor_view, SDL_Surface* surface) {
  int i = 0;
  for (i = 0; i < editor_view->toolbar->button_count; i++) {
    Sint16 thickness = 1;
    Sint16 red = 0;
    SDL_Rect* rect = &editor_view->toolbar->button[i].rect;
    SDL_BlitSurface(editor_view->toolbar->button[i].image, NULL, surface, rect);

    if (editor_view->toolbar->selected_button == i) {
      red = 255;
      thickness = 2;
    }
    draw_rectangle(surface, rect->x, rect->y, rect->x + rect->w,
                   rect->y + rect->h, red, 0, 0, 255, thickness, DRAW_OUT);
  }
}

// TODO: this could be more generic to allow drawing grid on any surface
static void draw_grid(EditorView* editor_view, SDL_Surface* surface,
                      SDL_Rect* destrect, unsigned int square_width,
                      unsigned int square_height) {
  int i = 0;
  SDL_Rect rect;
  SDL_Rect* p_rect;

  if (destrect != NULL) {
    p_rect = destrect;
  } else {
    rect.x = 0;
    rect.y = 0;
    rect.w = surface->w;
    rect.h = surface->h;
    p_rect = &rect;
  }

  for (i = 0; i < editor_view->map->column; i++) {
    lineRGBA(surface, p_rect->x + i * square_width, p_rect->y,
             p_rect->x + i * square_width, p_rect->y + p_rect->h, 0, 0, 0, 100);
  }

  for (i = 0; i < editor_view->map->row; i++) {
    lineRGBA(surface, p_rect->x, p_rect->y + i * square_height,
             p_rect->x + p_rect->w, p_rect->y + i * square_height, 0, 0, 0,
             100);
  }
}

Status editor_view_draw(EditorView* editor_view, SDL_Surface* surface) {
  int ret = 0;
  SDL_Rect map_rect;
  Status status = {MARIO_STATUS_ERROR, "unkonwn"};

  // TODO: move in event handler
  // WARNING: destrect's width/height should be well set
  // fill the window with white
  ret = SDL_FillRect(surface, NULL /*destrect*/,
                     SDL_MapRGB(surface->format, 255, 255, 255));
  if (ret != 0) {
    status.message = SDL_GetError();
    return status;
  }
  // draw the map
  map_view_draw(editor_view->map_view, surface, &map_rect);

  // draw a grid on map surface
  // TODO: map_rect should be embeded in map_view and set by constructor
  map_rect.x = 0;
  map_rect.y = 0;
  map_rect.w = map_view_get_width(editor_view->map_view);
  map_rect.h = map_view_get_height(editor_view->map_view);
  draw_grid(editor_view, surface, &map_rect,
            editor_view->map_view->square_width,
            editor_view->map_view->square_height);

  // draw a line to separate map view and toolbar
  lineRGBA(surface, map_rect.w, 0, map_rect.w, map_rect.h, 0, 0, 0, 100);

  // draw the toolbar
  draw_toolbar(editor_view, surface);

  status.code = MARIO_STATUS_SUCCESS;
  return status;
}

static bool get_toolbar_button_id(EditorView* view, unsigned int x,
                                  unsigned int y, unsigned int* button_id) {
  int i = 0;
  for (i = 0; i < view->toolbar->button_count; i++) {
    if (x < view->toolbar->button[i].rect.x ||
        x > view->toolbar->button[i].rect.x + view->toolbar->button[i].rect.w)
      continue;

    if (y < view->toolbar->button[i].rect.y ||
        y > view->toolbar->button[i].rect.y + view->toolbar->button[i].rect.h)
      continue;

    *button_id = i;
    return true;
  }
  return false;
}

static void event_left_button_up(EditorView* view, Game* game, unsigned int x,
                                 unsigned int y) {
  unsigned int button_id;
  debug("editor got left click (%d,%d)\n", x, y);

  // handle toolbar click
  if (get_toolbar_button_id(view, x, y, &button_id)) {
    debug("clicked button id %d\n", button_id);
    view->toolbar->selected_button = button_id;

    // TODO: toolbar surface should be hold in another rect
    //        if (SDL_FillRect(game->window->surface, &view->toolbar->rect,
    //                         SDL_MapRGB(game->window->surface->format,
    //                         255, 255,
    //                                    255) != 0)) {
    //          fprintf(stderr, "%s\n", SDL_GetError());
    //        }

    draw_toolbar(view, game->window->surface);

    SDL_Flip(game->window->surface);
  }

  // handle map view click
  // TODO: add a function into map_view.h

}

static void editor_view_event_handler(Game* game, const SDL_Event* event,
                                      void* param) {
  // debug("editor_event_handler\n");
  EditorView* view = (EditorView*)param;
  switch (event->type) {
    case SDL_MOUSEBUTTONUP:
      if (event->button.button != SDL_BUTTON_LEFT) {
        break;
      }
      event_left_button_up(view, game, event->button.x, event->button.y);
      break;
    default:
      break;
  }
}

void editor_view_get_event_handler(EditorView* view, EventHandler* handler) {
  handler->function = editor_view_event_handler;
  handler->param = view;
}
