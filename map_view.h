#ifndef MAP_VIEW_H_
#define MAP_VIEW_H_

#include <SDL/SDL.h>
#include <stdint.h>
#include <stdbool.h>

#include "status.h"

struct EventHandler;
struct Map;
struct Window;
struct Sprite;
struct Position;

typedef struct MapView {
  struct Map* map;
  struct Sprite** sprites;
  unsigned int sprite_count;
  unsigned int square_width;
  unsigned int square_height;
  SDL_Rect rect;
} MapView;

MapView* map_view_create(struct Map* map, const unsigned int surface_x,
                         const unsigned int surface_y);

void map_view_destroy(MapView* map_view);

Status map_view_draw(MapView* map_view, SDL_Surface* surface,
                     SDL_Rect* destrect);

unsigned int map_view_get_width(MapView* map_view);
unsigned int map_view_get_height(MapView* map_view);

bool map_view_get_position(MapView* map_view, const unsigned int x,
                           const unsigned int y, struct Position* position);

void map_view_get_event_handler(MapView* view, struct EventHandler* handler);

#endif  // MAP_VIEW_H_
