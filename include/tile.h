#pragma once
#include "stdafx.h"

class Tile
{
public:
  Tile(sf::Texture& texture_sheet, sf::IntRect texture_rect, bool damaging = false);
  void initSprite();
  void initTexture();

  const sf::FloatRect getGlobalBounds() const;
  void update();
  void render(sf::RenderTarget& target);

private:
  sf::Sprite sprite;
  const bool solid;
  sf::IntRect current_tile;
  sf::Texture texture_sheet;

};

