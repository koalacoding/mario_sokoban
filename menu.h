#ifndef MENU_H_
#define MENU_H_

#include "game.h"

#define MENU_ENTRY_MAX 10

typedef void (*MENU_ENTRY_CALLBACK)(void* param);
typedef struct {
    MENU_ENTRY_CALLBACK function;
    void* param;
} MenuEntryCallback;

typedef struct {
    const char* title;
    MenuEntryCallback callback;  // called on activation
} MenuEntry;

typedef struct Menu {
    MenuEntry entry[MENU_ENTRY_MAX];
    unsigned int entry_count;
} Menu;

Menu* menu_create();
void menu_destroy(Menu* menu);

void menu_add_entry(Menu* menu, const char* title,
                    MENU_ENTRY_CALLBACK callback, void* callback_param);
//void menu_select_entry(Menu* menu, unsigned int entry_id);

// EventHandler prototype
void menu_event_handler(const SDL_Event* event, void* param);

#endif  // MENU_H_
