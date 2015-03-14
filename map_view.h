#ifndef MAP_VIEW_H_
#define MAP_VIEW_H_

#include <SDL/SDL.h>
#include <stdint.h>

#include "status.h"

struct EventHandler;
struct Map;
struct Window;
struct Sprite;

typedef struct MapView {
  struct Map* map;
  struct Sprite** sprites;
  unsigned int sprite_count;
  unsigned int square_width;
  unsigned int square_height;
} MapView;

MapView* map_view_create(struct Map* map);

void map_view_destroy(MapView* map_view);

Status map_view_draw(MapView* map_view, SDL_Surface* surface,
                     SDL_Rect* destrect);

unsigned int map_view_get_height(MapView* map_view);
unsigned int map_view_get_width(MapView* map_view);

void map_view_get_event_handler(MapView* view, struct EventHandler* handler);

#endif  // MAP_VIEW_H_
