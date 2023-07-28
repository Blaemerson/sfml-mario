#pragma once
#include "stdafx.h"
#include "player.hpp"
#include "textbox.hpp"
#include "tilemap.hpp"
#include "window.hpp"

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
  void update(sf::Time dt);
  void processEvents();
  void render();
  void updateView();
  void updateColision();
  void renderPlayer();
  void renderMap();

  sf::Time restartClock();
private:
  // Variables / window
  // sf::RenderWindow window;
  Window window;
  Textbox textbox;

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

