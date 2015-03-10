#include "window.h"

Window* window_create(const char* caption, const unsigned int width,
                      const unsigned int height) {
  Window* new_window = NULL;
  int ret = -1;

  Window* window = malloc(sizeof(Window));
  if (window == NULL) {
    fprintf(stderr, "Cannot allocate window %s\n", caption);
    goto end;
  }

  // CAREFULL! The surface returned is freed by SDL_Quit() and should'nt be
  // freed by the caller
  window->surface =
      SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (window->surface == NULL) {
    fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
    goto end;
  }

  SDL_WM_SetCaption(caption, NULL);

  // fill the window with white
  ret = SDL_FillRect(window->surface, NULL,
                     SDL_MapRGB(window->surface->format, 255, 255, 255));
  if (ret != 0) {
    fprintf(stderr, "SDL_FillRect failed on %s\n", SDL_GetError());
    goto end;
  }

  // as the window as SDL_DOUBLEBUF flag, it should be flipped each time
  // the changes have to be draw (it flip the buffers)
  ret = SDL_Flip(window->surface);
  if (ret != 0) {
    fprintf(stderr, "Failed to swap the buffers: %s\n", SDL_GetError());
    goto end;
  }

  // no error, new_window can be defined and returned
  new_window = window;

end:
  // only cleanup if anything went wrong
  if (new_window == NULL) {
    if (window != NULL) {
      free(window);
    }
  }
  return new_window;
}

void window_destroy(Window* window) {
  // SDL_FreeSurface(window->surface);  // DON'T !!! See SDL_SetVideoMode()
  free(window);
}
