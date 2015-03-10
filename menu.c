#include "menu.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "status.h"

Menu* menu_create() {
  Menu* menu, * new_menu = NULL;

  menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    return NULL;
  }
  memset(menu, 0, sizeof(Menu));

  new_menu = menu;
end:
  if (new_menu == NULL) {
    menu_destroy(menu);
  }
  return new_menu;
}

void menu_destroy(Menu* menu) {}

void menu_add_entry(Menu* menu, const char* title, MENU_ENTRY_CALLBACK callback,
                    void* callback_param) {
  assert(menu->entry_count < MENU_ENTRY_MAX);
  menu->entry[menu->entry_count].callback.function = callback;
  menu->entry[menu->entry_count].callback.param = callback_param;
  menu->entry[menu->entry_count].title = title;
  menu->entry_count++;
}

void menu_select_entry(Menu* menu, unsigned int entry_id) {
  fprintf(stderr, "selected entry %d\n", entry_id);
}

void menu_event_handler(const SDL_Event* event, void* param) {
  // debug("menu_event_handler\n");
  Menu* menu = (Menu*)param;
  switch (event->type) {
    case SDL_KEYDOWN:
      debug("menu got %d\n", event->key.keysym.sym);
      switch (event->key.keysym.sym) {
        case SDLK_UP:
          break;
        case SDLK_DOWN:
          break;
      }
  }
}

// EventHandler menu_get_event_handler(Menu* menu) {
//    EventHandler event_handler;
//    event_handler.function = menu_event_handler;
//    event_handler.param = menu;
//    return event_handler;
//}
