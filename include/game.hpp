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

  Window* GetWindow();

  void Update(sf::Time dt);
  void ProcessEvents();
  void Render();
  void UpdateView();
  void UpdateCollision();
  void RenderPlayer();
  void RenderTileMap();

  sf::Time RestartClock();

private:
  void InitializeWindow();
  void InitializeView();
  void InitializePlayer();
  void InitializeMap();

private:
  Window window;
  Textbox textbox;

  sf::View view;

  sf::Clock clock;

  Player* player;
  TileMap* tilemap;
};

