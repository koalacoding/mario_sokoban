#include "game.h"

// STANDARD HEADERS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// EXTERNAL HEADERS
#include <SDL/SDL.h>

// CUSTOM HEADERS
#include "status.h"
#include "window.h"
#include "map.h"
#include "map_view.h"
#include "menu_view.h"

// divide per 2 to test window resizing
#define GAME_WINDOW_DEFAULT_WIDTH 408/2
#define GAME_WINDOW_DEFAULT_HEIGHT 408/2

static const char* map_default = "maps/map0.map";

Game* game_create() {
  Game* game;
  Game* new_game = NULL;

  game = malloc(sizeof(Game));
  if (game == NULL) {
    fprintf(stderr, "cannot allocate Game\n");
    goto end;
  }
  memset(game, 0, sizeof(Game));
  game->map_file = map_default;

  // create the main menu
  game->menu = menu_create();
  if (game->menu == NULL) {
    goto end;
  }
  menu_add_entry(game->menu, "Play", (MENU_ENTRY_CALLBACK)game_go_play, game);
  menu_add_entry(game->menu, "Load level", (MENU_ENTRY_CALLBACK)game_go_load,
                 game);
  menu_add_entry(game->menu, "Create level",
                 (MENU_ENTRY_CALLBACK)game_go_editor, game);
  menu_add_entry(game->menu, "Exit", (MENU_ENTRY_CALLBACK)game_go_exit, game);

  game->menu_view = menu_view_create(game->menu, 50, 100);
  if (game->menu_view == NULL) {
    goto end;
  }

  }

  game->window = window_create("Mario Sokoban", GAME_WINDOW_DEFAULT_WIDTH,
                               GAME_WINDOW_DEFAULT_HEIGHT);
  if (game->window == NULL) {
    fprintf(stderr, "Cannot create main window\n");
    goto end;
  }
  new_game = game;
end:
  if (new_game == NULL) {
    game_destroy(game);
  }
  return new_game;
}

void game_destroy(Game* game) {
  if (game->map_view) {
    map_view_destroy(game->map_view);
    game->map_view = NULL;
  }
  if (game->map) {
    map_destroy(game->map);
    game->map = NULL;
  }
  if (game->menu_view) {
    menu_view_destroy(game->menu_view);
    game->menu_view = NULL;
  }
  if (game->menu) {
    menu_destroy(game->menu);
    game->menu = NULL;
  }
  if (game->window) {
    window_destroy(game->window);
    game->window = NULL;
  }
  free(game);
}

static void execute_event_handlers(Game* game, const SDL_Event* event) {
  if (game->event_handler.function) {
    game->event_handler.function(game, event, game->event_handler.param);
  }
}

void game_set_event_handler(Game* game, EventHandler event_handler) {
  game->event_handler = event_handler;
}

int game_run(Game* game) {
  SDL_Event event;
  bool exit_event_loop = false;

  // display the menu first
  game_go_menu(game);

  while (exit_event_loop == false) {
    SDL_WaitEvent(&event);
    // There's one SDL event loop so all event will be processed by this
    // switch before being by custom handler (see execute_event_handler)
    switch (event.type) {
      case SDL_QUIT:
        exit_event_loop = true;
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            game_go_menu(game);
            break;
        }
      default:
        execute_event_handlers(game, &event);
    }
  }
  return 0;
}

void game_go_menu(Game* game) {
  EventHandler handler;
  menu_view_get_event_handler(game->menu_view, &handler);
  game_set_event_handler(game, handler);

  // resize the window to the view size
  game->window->surface = SDL_SetVideoMode(menu_view_get_width(game->menu_view),
                                           menu_view_get_width(game->menu_view),
                                           32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (game->window->surface == NULL) {
    fprintf(stderr, "Window resizing failed: %s\n", SDL_GetError());
  }
  // WARNING: destrect's size should be well set to fill the window with white
  if (SDL_FillRect(game->window->surface, NULL /*destrect*/,
                   SDL_MapRGB(game->window->surface->format, 255, 255, 255)) !=
      0) {
    fprintf(stderr, "%s\n", SDL_GetError());
  }
  menu_view_draw(game->menu_view, game->window->surface);
  SDL_Flip(game->window->surface);
}

void game_go_play(Game* game) {
  Status status;

  debug("game_go_play\n");

  // cleanup any previous map stuff
  if (game->map_view) {
    map_view_destroy(game->map_view);
    game->map_view = NULL;
  }
  if (game->map) {
    map_destroy(game->map);
    game->map = NULL;
  }

  // create the new ones
  if (game->map == NULL) {
    game->map = map_create(game->map_file);
    if (game->map == NULL) {
      goto end;
    }
  }

  if (game->map_view == NULL) {
    game->map_view = map_view_create(game->map);
    if (game->map_view == NULL) {
      goto end;
    }
  }

  // resize the window to the view size
  game->window->surface = SDL_SetVideoMode(map_view_get_width(game->map_view),
                                           map_view_get_height(game->map_view),
                                           32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (game->window->surface == NULL) {
    fprintf(stderr, "Window resizing failed: %s\n", SDL_GetError());
    goto end;
  }

  status = map_view_draw(game->map_view, game->window->surface, NULL);
  if (status.code != MARIO_STATUS_SUCCESS) {
    goto end;
  }

  struct EventHandler handler;
  map_view_get_event_handler(game->map_view, &handler);
  game_set_event_handler(game, handler);
  SDL_Flip(game->window->surface);
end:
  return;
}

void game_go_load(Game* game) { debug("game_go_load\n"); }

void game_go_editor(Game* game) { debug("game_go_editor\n"); }

void game_go_exit(Game* game) {
  debug("game_go_exit\n");
  exit(0);
}
