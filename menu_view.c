#include "menu_view.h"

MenuView* menu_view_create(Menu* menu, unsigned x, unsigned y) {
  MenuView* view, * new_view = NULL;
  const SDL_Color default_color = {127, 127, 127};

  view = malloc(sizeof(MenuView));
  if (view == NULL) {
    return NULL;
  }
  memset(view, 0, sizeof(MenuView));
  view->menu = menu;
  view->x = x;
  view->y = y;
  view->font_size = 30;
  view->font_color = default_color;

  // TODO: TTF_Init() should be globally initialized
  if (TTF_WasInit() == 0 && TTF_Init() == -1) {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    goto end;
  }

  view->font = TTF_OpenFont("fonts/ubuntu-b.ttf", view->font_size);
  if (view->font == NULL) {
    fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
    goto end;
  }

  new_view = view;
end:
  if (new_view == NULL) {
    menu_view_destroy(view);
  }
  return new_view;
}

void menu_view_destroy(MenuView* view) {
  if (view->font != NULL) {
    TTF_CloseFont(view->font);
    view->font = NULL;
  }

  // TODO: TTF_Init() should be globally initialized
  if (TTF_WasInit()) {
    TTF_Quit();
  }
  free(view);
}

unsigned int menu_view_get_width(MenuView* menu_view) {
  return 408;
}

unsigned int menu_view_get_height(MenuView* menu_view) {
  return 408;
}

void menu_view_draw(MenuView* view, SDL_Surface* surface) {
  Menu* menu = view->menu;
  unsigned int i = 0;

  // SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 255, 255));

  for (i = 0; i < menu->entry_count; i++) {
    SDL_Surface* text_surface = NULL;
    SDL_Rect text_rect;
    const MenuEntry* entry = &menu->entry[i];
    text_surface =
        TTF_RenderUTF8_Blended(view->font, entry->title, view->font_color);
    text_rect.x = view->x;
    text_rect.y = view->y + i * view->font_size;
    SDL_BlitSurface(text_surface, NULL, surface, &text_rect);
    SDL_FreeSurface(text_surface);
  }
}

static MenuEntry* get_entry(MenuView* view, unsigned int x, unsigned int y) {
  int view_x = 0;  // x with menu as origin
  int view_y = 0;  // y with menu as origin
  unsigned int row = 0;

  // switch to view menu origin
  view_x = x - view->x;
  view_y = y - view->y;

  // debug("get_menu_entry (%d,%d) => (%d,%d)\n", x, y, view_x, view_y);

  // check if x,y are in menu view limits
  if (view_x < 0 || view_y < 0) {
    return NULL;
  }

  row = view_y / view->font_size;
  // check a menu entry exists
  if (row > view->menu->entry_count - 1) {
    return NULL;
  }

  // TODO: check x bounds

  debug("(%d,%d) => menu entry %d\n", x, y, row);
  return &view->menu->entry[row];
}

static void menu_view_event_handler(Game* game, const SDL_Event* event,
                                    void* param) {
  // debug("menu_event_handler\n");
  MenuView* view = (MenuView*)param;
  switch (event->type) {
    case SDL_KEYDOWN:
      debug("menu got %d\n", event->key.keysym.sym);
      switch (event->key.keysym.sym) {
        case SDLK_UP:
          break;
        case SDLK_DOWN:
          break;
      }
    case SDL_MOUSEBUTTONUP:
      if (event->button.button != SDL_BUTTON_LEFT) {
        break;
      }
      // get the clicked menuentry
      MenuEntry* entry = get_entry(view, event->button.x, event->button.y);
      if (entry == NULL) {
        break;
      }
      // call the associated function
      entry->callback.function(entry->callback.param);
      break;
    default:
      // forward unprocessed events to menu's event handler
      menu_event_handler(event, view->menu);
      break;
  }
}

void menu_view_get_event_handler(MenuView* view, EventHandler* handler) {
  handler->function = menu_view_event_handler;
  handler->param = view;
}
