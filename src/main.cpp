#include "../include/stdafx.h"
#include "../include/game.hpp"
// #include "../include/tilemap.h"

int main(int argc, char** argv)
{
  // srand(static_cast<unsigned>(time(0)));

  // Init game engine
  Game game;

  // Game loop
  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  sf::Time timePerFrame = sf::seconds(1.f / 60.f);

  while(!game.GetWindow()->isDone()) {
    game.ProcessEvents();

    timeSinceLastUpdate += game.RestartClock();
    while (timeSinceLastUpdate > timePerFrame) {
      timeSinceLastUpdate -= timePerFrame;
      game.Update(timePerFrame);
    }

    game.Render();
  }

  // End
  return 0;
}
