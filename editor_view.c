#include "editor_view.h"

#include <SDL_gfxPrimitives.h>

#include "game.h"
#include "window.h"
#include "sprite.h"
#include "map.h"
#include "map_view.h"
#include "toolbar.h"

EditorView* editor_view_create(const SDL_Color background_color) {
  EditorView* editor = NULL;
  EditorView* new_editor = NULL;
  const int x_offset = 1;
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

  editor->map_view = map_view_create(editor->map, 0, 0);
  if (editor->map_view == NULL) {
    goto end;
  }

  // TODO: is x_offset ugly ?
  editor->background_color = background_color;
  editor->tools_rect.x = map_view_get_width(editor->map_view);
  editor->tools_rect.y = 0;
  editor->toolbar = toolbar_create(editor->tools_rect.x + x_offset,
                                   editor->tools_rect.y, 10, background_color);
  if (editor->toolbar == NULL) {
    fprintf(stderr, "cannot create toolbar\n");
    goto end;
  }

  // TODO: ask the sprites to a sprite factory, don't use map_view
  for (i = 0; i < editor->map_view->sprite_count; i++) {
    toolbar_add_button(editor->toolbar,
                       editor->map_view->sprites[i]->image[DIRECTION_DOWN],
                       TOOLBAR_BUTTON_ATTRIBUTE_SELECT, NULL, NULL);
  }
  editor->tools_rect.w = toolbar_get_width(editor->toolbar) + x_offset;
  editor->tools_rect.h = map_view_get_height(editor->map_view);

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
    toolbar_destroy(editor_view->toolbar);
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
  return map_view_get_width(editor_view->map_view) + editor_view->tools_rect.w;
}

unsigned int editor_view_get_height(EditorView* editor_view) {
  return map_view_get_height(editor_view->map_view);
}

static void draw_grid(SDL_Surface* surface, SDL_Rect* destrect,
                      unsigned int square_width, unsigned int square_height) {
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

  for (i = 0; i <= p_rect->w / square_width; i++) {
    lineRGBA(surface, p_rect->x + i * square_width, p_rect->y,
             p_rect->x + i * square_width, p_rect->y + p_rect->h, 0, 0, 0, 100);
  }

  for (i = 0; i <= p_rect->h / square_height; i++) {
    lineRGBA(surface, p_rect->x, p_rect->y + i * square_height,
             p_rect->x + p_rect->w, p_rect->y + i * square_height, 0, 0, 0,
             100);
  }
}

Status editor_view_draw(EditorView* editor_view, SDL_Surface* surface) {
  int ret = 0;
  SDL_Rect* map_rect = &editor_view->map_view->rect;
  Status status = {MARIO_STATUS_ERROR, "unkonwn"};

  // tools background
  ret =
      SDL_FillRect(surface, &editor_view->tools_rect,
                   SDL_MapRGB(surface->format, editor_view->background_color.r,
                              editor_view->background_color.g,
                              editor_view->background_color.b));
  if (ret != 0) {
    fprintf(stderr, "editor_view failed to draw background: %s\n",
            SDL_GetError());
  }

  // draw the map
  map_view_draw(editor_view->map_view, surface, map_rect);

  // draw a grid on map surface
  draw_grid(surface, map_rect, editor_view->map_view->square_width,
            editor_view->map_view->square_height);

  // draw a line to separate map view and toolbar
  lineRGBA(surface, map_rect->w, 0, map_rect->w, map_rect->h, 0, 0, 0, 255);

  // draw the toolbar
  toolbar_draw(editor_view->toolbar, surface);

  status.code = MARIO_STATUS_SUCCESS;
  return status;
}

static void event_left_button_up(EditorView* editor_view, Game* game,
                                 unsigned int x, unsigned int y) {
  unsigned int button_id;
  Position position;
  debug("editor got left click (%d,%d)\n", x, y);

  // handle toolbar click
  if (toolbar_get_button_id(editor_view->toolbar, x, y, &button_id)) {
    debug("clicked button id %d\n", button_id);
    toolbar_trigger_button(editor_view->toolbar, button_id);
    toolbar_draw(editor_view->toolbar, game->window->surface);
    SDL_Flip(game->window->surface);
    return;
  }

  // TODO: drawing stuff is too fat
  // handle map view click
  if (map_view_get_position(editor_view->map_view, x, y, &position)) {
    Status status;
    debug("clicked position (%d,%d)\n", position.x, position.y);
    map_set_square(editor_view->map, position.x, position.y,
                   editor_view->toolbar->selected_button_id, DIRECTION_DOWN,
                   &status);
    map_view_draw(editor_view->map_view, game->window->surface,
                  &editor_view->map_view->rect);
    draw_grid(game->window->surface, &editor_view->map_view->rect,
              editor_view->map_view->square_width,
              editor_view->map_view->square_height);
    SDL_Flip(game->window->surface);
    return;
  }
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
