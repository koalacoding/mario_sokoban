#ifndef MENU_VIEW_H_
#define MENU_VIEW_H_

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "menu.h"

typedef struct MenuView {
  struct Menu* menu;
  unsigned int x;
  unsigned int y;
  TTF_Font* font;
  unsigned int font_size;
  SDL_Color font_color;
} MenuView;

MenuView* menu_view_create(Menu* menu, unsigned x, unsigned y);
void menu_view_destroy(MenuView* menu_view);

unsigned int menu_view_get_width(MenuView* menu_view);
unsigned int menu_view_get_height(MenuView* menu_view);

void menu_view_draw(MenuView* menu_view, SDL_Surface* surface);

void menu_view_get_event_handler(MenuView* view, EventHandler* handler);

#endif  // MENU_VIEW_H_
