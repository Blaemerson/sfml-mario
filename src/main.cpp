#include "../include/stdafx.h"
#include "../include/game.h"
// #include "../include/tilemap.h"

int main(int argc, char** argv)
{
  // srand(static_cast<unsigned>(time(0)));

  // Init game engine
  Game game;

  // Game loop
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  sf::Time timePerFrame = sf::seconds(1.f / 60.f);
  while(!game.getWindow()->isDone()) {
    game.processEvents();

    timeSinceLastUpdate += game.restartClock();
    while (timeSinceLastUpdate > timePerFrame) {
      timeSinceLastUpdate -= timePerFrame;
      game.update(timePerFrame);
    }

    game.render();
  }

  // End
  return 0;
}
