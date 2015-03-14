#include "game.h"

#include <SDL/SDL_ttf.h>

int main(int argc, char* argv[]) {
  int exit_code = 0;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  if (TTF_Init() != 0) {
    fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
    SDL_Quit();
    return 1;
  }

  Game* mario_sokoban = game_create();
  if (mario_sokoban == NULL) {
    return 1;
  }
  exit_code = game_run(mario_sokoban);
  game_destroy(mario_sokoban);

  TTF_Quit();
  SDL_Quit();

  return exit_code;
}
