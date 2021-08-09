#include "../include/stdafx.h"
#include "../include/game.h"

int main(int argc, char** argv)
{
  // srand(static_cast<unsigned>(time(0)));

  // Init game engine
  Game game;

  // Game loop
  while(game.getWindow().isOpen())
  {
    game.update();
    game.render();
    game.restartClock();
  }

  // End
  return 0;
}
