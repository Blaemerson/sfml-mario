#pragma once
#include "stdafx.h"
#include "player.h"
#include "tilemap.h"
#include "window.h"

/* Wrapper class */

class Game
{
public:
  // Consructor / Destructor
  Game();
  virtual ~Game();

  // Accessors
  Window* getWindow();

  // Methods
  void update(sf::Time delta_time);
  void processEvents();
  void render();
  void updateView();
  void updateColision();
  void renderPlayer();
  void renderMap();

  void debugPrint(int player_index);

  sf::Time restartClock();
private:
  // Variables / window
  // sf::RenderWindow window;
  Window* window;
  sf::View view;

  sf::Clock clock;

  // Game objects
  Player* player;
  TileMap* tilemap;

  // Private methods
  void initWindow();
  void initView();
  void initPlayer();
  void initMap();
};

