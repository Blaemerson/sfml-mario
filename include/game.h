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
  sf::RenderWindow window;
  sf::VideoMode video_mode;
  sf::Event event;
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

