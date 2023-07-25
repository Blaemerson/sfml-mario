#pragma once
#include "stdafx.h"
#include "window.hpp"

class TileMap: public sf::Drawable, public sf::Transformable
{
public:
    TileMap(const std::string& tilesheet, const int &tiles, const unsigned int height, const unsigned int width, const unsigned int tileSize);
    bool initTexture(const std::string& tilesheet);
    bool load();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void draw(Window& window);
    const sf::FloatRect getTile(int pos);
private:

    unsigned int height;
    unsigned int width;
    unsigned int tile_size;
    sf::VertexArray vertices;
    const int* tiles;
    sf::Texture tileset;
};
