#ifndef EDITOR_VIEW_H_
#define EDITOR_VIEW_H_

#include <SDL/SDL.h>

#include "status.h"

struct Sprite;
struct Map;
struct MapView;
struct Toolbar;
struct EventHandler;

typedef struct EditorView {
  struct Map* map;
  struct MapView* map_view;
  struct Toolbar* toolbar;
  SDL_Rect tools_rect;
} EditorView;

EditorView* editor_view_create();

void editor_view_destroy(EditorView* editor_view);

unsigned int editor_view_get_width(EditorView* editor_view);
unsigned int editor_view_get_height(EditorView* editor_view);

Status editor_view_draw(EditorView* editor_view, SDL_Surface* surface);

void editor_view_get_event_handler(EditorView* view,
                                   struct EventHandler* handler);

#endif  // EDITOR_VIEW_H_
