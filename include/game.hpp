#pragma once
#include "stdafx.h"
#include "player.hpp"
#include "textbox.hpp"
#include "tilemap.hpp"
#include "window.hpp"

class Game
{
public:
  Game();
  virtual ~Game();

  Window* getWindow();

  void update(sf::Time dt);
  void processEvents();
  void render();
  void updateView();
  void updateColision();
  void renderPlayer();
  void renderMap();

  sf::Time restartClock();

private:
  void initWindow();
  void initView();
  void initPlayer();
  void initMap();

private:
  Window window;
  Textbox textbox;

  sf::View view;

  sf::Clock clock;

  Player* player;
  TileMap* tilemap;
};

