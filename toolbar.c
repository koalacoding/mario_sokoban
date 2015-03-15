#include "toolbar.h"

typedef enum { DRAW_IN, DRAW_OUT } DRAW_SIDE;

Toolbar* toolbar_create(const unsigned int surface_x,
                        const unsigned int surface_y,
                        const unsigned int button_spacing,
                        const SDL_Color background_color) {
  Toolbar* toolbar = NULL;
  Toolbar* new_toolbar = NULL;

  toolbar = malloc(sizeof(Toolbar));
  if (toolbar == NULL) {
    fprintf(stderr, "Toolbar allocation failed\n");
    goto end;
  }
  memset(toolbar, 0, sizeof(Toolbar));

  toolbar->rect.x = surface_x;
  toolbar->rect.y = surface_y;
  toolbar->button_count = 0;
  toolbar->button_spacing = button_spacing;
  toolbar->button_margin = 10;
  toolbar->selected_button_id = 0;
  toolbar->background_color = background_color;

  new_toolbar = toolbar;
end:
  if (new_toolbar == NULL) {
    toolbar_destroy(toolbar);
  }
  return new_toolbar;
}

void toolbar_destroy(Toolbar* toolbar) { free(toolbar); }

unsigned int toolbar_get_width(Toolbar* toolbar) { return toolbar->rect.w; }

unsigned int toolbar_get_height(Toolbar* toolbar) { return toolbar->rect.x; }

static void update_toolbar_rect(Toolbar* toolbar) {
  int i;
  unsigned int button_max_width = 0;
  unsigned int button_total_height = 0;

  // get the biggest button width and sum the button heights
  for (i = 0; i < toolbar->button_count; i++) {
    if (toolbar->button[i].rect.w > button_max_width) {
      button_max_width = toolbar->button[i].rect.w;
    }
    button_total_height += toolbar->button[i].rect.h;
  }
  button_total_height += toolbar->button_count * toolbar->button_spacing;

  toolbar->rect.w = button_max_width + toolbar->button_margin * 2;
  toolbar->rect.h = button_total_height + toolbar->button_margin;
}

void toolbar_add_button(Toolbar* toolbar, SDL_Surface* image,
                        int attribute_mask, TOOLBAR_BUTTON_CALLBACK callback,
                        void* callback_param) {
  unsigned int margin_top = 0;
  if (toolbar->button_count > TOOLBAR_BUTTON_MAX) {
    fprintf(stderr, "cannot add button, toolbar is full\n");
    return;
  }
  if (toolbar->button_count == 0) {
    margin_top = toolbar->button_margin;
  }
  ToolbarButton* button = &toolbar->button[toolbar->button_count];

  button->image = image;
  button->rect.x = toolbar->rect.x + toolbar->button_margin;
  button->rect.y = toolbar->rect.y + toolbar->rect.h + margin_top;
  button->rect.w = image->w;
  button->rect.h = image->h;
  button->selectable = attribute_mask & TOOLBAR_BUTTON_ATTRIBUTE_SELECT;
  button->callback.function = callback;
  button->callback.param = callback_param;

  toolbar->button_count++;

  // ensure toolbar's rect has always the correct size
  update_toolbar_rect(toolbar);
}

static draw_rectangle(SDL_Surface* surface, Sint16 x1, Sint16 y1, Sint16 x2,
                      Sint16 y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                      const Sint16 thickness, const DRAW_SIDE draw_side) {
  int i;
  if (draw_side == DRAW_IN) {
    for (i = 0; i < thickness; i++) {
      rectangleRGBA(surface, x1 + i, y1 + i, x2 - i, y2 - i, r, g, b, a);
    }
  } else {
    for (i = 0; i < thickness; i++) {
      rectangleRGBA(surface, x1 - i - 1, y1 - i - 1, x2 + i, y2 + i, r, g, b,
                    a);
    }
  }
}

void toolbar_draw(Toolbar* toolbar, SDL_Surface* surface) {
  int i = 0;
  int ret = 0;
  // draw toolbar background
  ret = SDL_FillRect(
      surface, &toolbar->rect,
      SDL_MapRGB(surface->format, toolbar->background_color.r,
                 toolbar->background_color.g, toolbar->background_color.b));
  if (ret != 0) {
    fprintf(stderr, "draw_toolbar failed to draw background: %s\n",
            SDL_GetError());
  }

  for (i = 0; i < toolbar->button_count; i++) {
    Sint16 thickness = 1;
    Sint16 red = 0;
    SDL_Rect* rect = &toolbar->button[i].rect;

    // draw sprites background
    ret =
        SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, 255, 255, 255));
    if (ret != 0) {
      fprintf(stderr, "draw_toolbar failed to draw button background: %s\n",
              SDL_GetError());
    }

    SDL_BlitSurface(toolbar->button[i].image, NULL, surface, rect);

    // draw selection and buttons border
    if (toolbar->selected_button_id == i) {
      red = 255;
      thickness = 2;
    }
    draw_rectangle(surface, rect->x, rect->y, rect->x + rect->w,
                   rect->y + rect->h, red, 0, 0, 255, thickness, DRAW_OUT);
  }
}

bool toolbar_get_button_id(Toolbar* toolbar, const unsigned int x,
                           const unsigned int y, unsigned int* button_id) {
  int i = 0;
  for (i = 0; i < toolbar->button_count; i++) {
    if (x < toolbar->button[i].rect.x ||
        x > toolbar->button[i].rect.x + toolbar->button[i].rect.w)
      continue;

    if (y < toolbar->button[i].rect.y ||
        y > toolbar->button[i].rect.y + toolbar->button[i].rect.h)
      continue;

    *button_id = i;
    return true;
  }
  return false;
}

// draw button selection and run the associated callback, surface can be NULL
void toolbar_trigger_button(Toolbar* toolbar, const unsigned int button_id) {
  const ToolbarButton* button = &toolbar->button[button_id];
  if (button->selectable) {
    toolbar->selected_button_id = button_id;
  }
  if (button->callback.function) {
    button->callback.function(button->callback.param);
  }
}
