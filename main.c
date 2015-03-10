#include "game.h"

int main(int argc, char* argv[]) {
  int exit_code = 0;
  Game* mario_sokoban = game_create();
  if (mario_sokoban == NULL) {
    return 1;
  }
  exit_code = game_run(mario_sokoban);
  game_destroy(mario_sokoban);
  return exit_code;
}
