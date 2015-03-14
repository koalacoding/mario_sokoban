#include "editor_view.h"

#include <SDL_gfxPrimitives.h>

#include "game.h"
#include "sprite.h"
#include "map.h"
#include "map_view.h"

typedef struct Toolbar {
  SDL_Rect rect;  // position in the 'parent' surface
  struct ToolbarButton* button;
  unsigned int button_count;
  unsigned int button_width;
  unsigned int button_height;
} Toolbar;

typedef struct ToolbarButton {
  SDL_Rect rect;  // position in the toolbar's 'parent' surface
  SDL_Surface* image;
} ToolbarButton;

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
    editor->toolbar->rect.y = 0;
    editor->toolbar->rect.w = 50;
    editor->toolbar->rect.h = map_view_get_height(editor->map_view);
    editor->toolbar->button[i].rect.x = 10 + editor->toolbar->rect.x;
    editor->toolbar->button[i].rect.y =
        (10 + square_height) * i + editor->toolbar->rect.y;
    editor->toolbar->button[i].rect.w = square_width;
    editor->toolbar->button[i].rect.h = square_height;
  }

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

static void draw_toolbar(EditorView* editor_view, SDL_Surface* surface) {
  int i = 0;
  lineRGBA(surface, editor_view->toolbar->rect.x, editor_view->toolbar->rect.y,
           editor_view->toolbar->rect.x, editor_view->toolbar->rect.h, 0, 0, 0,
           100);
  for (i = 0; i < SQUARE_ID_COUNT; i++) {
    SDL_BlitSurface(editor_view->toolbar->button[i].image, NULL, surface,
                    &editor_view->toolbar->button[i].rect);
    continue;
  }
}

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

  // draw the toolbar
  draw_toolbar(editor_view, surface);

  // TODO: move into the event handler
  SDL_Flip(surface);
  status.code = MARIO_STATUS_SUCCESS;
  return status;
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
      debug("editor got left click (%d,%d)", event->button.x, event->button.y);
      break;
    default:
      break;
  }
}

void editor_view_get_event_handler(EditorView* view, EventHandler* handler) {
  handler->function = editor_view_event_handler;
  handler->param = view;
}
