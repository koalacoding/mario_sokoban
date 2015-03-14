#ifndef GAME_H_
#define GAME_H_

#include <SDL/SDL.h>

struct Game;

typedef void (*EVENT_HANDLER_FUNCTION)(struct Game* game, const SDL_Event*,
                                       void* param);

typedef struct EventHandler {
  EVENT_HANDLER_FUNCTION function;
  void* param;
} EventHandler;

struct Window;
struct Menu;
struct MenuView;
struct Map;
struct MapView;

typedef struct Game {
  struct Window* window;
  struct Menu* menu;
  struct MenuView* menu_view;
  struct Map* map;
  struct MapView* map_view;
  struct EditorView* editor_view;
  EventHandler event_handler;
  const char* map_file;
} Game;

Game* game_create();
void game_destroy(Game* game);

int game_run(Game* game);
void game_set_event_handler(Game* game, EventHandler event_handler);

void game_go_menu(Game* game);    // show the game menu
void game_go_play(Game* game);    // play the current level
void game_go_load(Game* game);    // load a level
void game_go_editor(Game* game);  // show the editor
void game_go_exit(Game* game);

#endif  // GAME_H_
