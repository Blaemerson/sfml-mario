#pragma once
#include "stdafx.h"
#include "player.h"
#include "tilemap.h"

/* Wrapper class */

class Game
{
public:
  // Consructor / Destructor
  Game();
  virtual ~Game();

  // Accessors
  const sf::RenderWindow& getWindow() const;

  // Methods
  void update();
  void render();
  void updatePlayer();
  void updateView();
  void updateColision();
  void renderPlayer();
  void renderMap();

  sf::Time getElapsed();
  void restartClock();
private:
  // Variables / window
  sf::RenderWindow window;
  sf::VideoMode video_mode;
  sf::Event event;
  sf::View view;

  float dt = 0.f;
  sf::Clock m_clock;
  sf::Time m_elapsed;

  // Game objects
  Player* player;
  TileMap* tilemap;

  // Private methods
  void initWindow();
  void initView();
  void initPlayer();
  void initMap();
};

