#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <stdbool.h>

#include <SDL/SDL.h>

#define TOOLBAR_BUTTON_MAX 10

#define TOOLBAR_BUTTON_ATTRIBUTE_SELECT 0x00000001

typedef void (*TOOLBAR_BUTTON_CALLBACK)(void* param);
typedef struct ToolbarButtonCallback {
  TOOLBAR_BUTTON_CALLBACK function;
  void* param;
} ToolbarButtonCallback;

typedef struct ToolbarButton {
  SDL_Rect rect;  // position in the toolbar's 'parent' surface
  SDL_Surface* image;
  int selectable;
  ToolbarButtonCallback callback;
} ToolbarButton;

typedef struct Toolbar {
  SDL_Rect rect;  // position in the 'parent' surface
  ToolbarButton button[TOOLBAR_BUTTON_MAX];
  unsigned int button_count;
  unsigned int button_width;
  unsigned int button_height;
  unsigned int button_spacing;
  unsigned int selected_button_id;
} Toolbar;

Toolbar* toolbar_create(const unsigned int surface_x,
                        const unsigned int surface_y,
                        const unsigned int button_spacing);
void toolbar_destroy(Toolbar* toolbar);

unsigned int toolbar_get_width(Toolbar* toolbar);
unsigned int toolbar_get_height(Toolbar* toolbar);

void toolbar_add_button(Toolbar* toolbar, SDL_Surface* image,
                        int attribute_mask, TOOLBAR_BUTTON_CALLBACK callback, void* callback_param);

void toolbar_draw(Toolbar* toolbar, SDL_Surface* surface);

bool toolbar_get_button_id(Toolbar* toolbar, const unsigned int x,
                           const unsigned int y, unsigned int* button_id);

void toolbar_trigger_button(Toolbar* toolbar, const unsigned int button_id);

#endif  // TOOLBAR_H_
