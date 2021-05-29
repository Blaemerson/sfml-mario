#pragma once
#include "tile.h"

class TileMap
{
private:
  std::vector<std::vector<Tile*>> tiles;

public:
  TileMap();
  ~TileMap();

  void addTile(unsigned x, unsigned y);
  void removeTile(unsigned x, unsigned y);

  void update();
  void render(sf::RenderTarget& target);
};

