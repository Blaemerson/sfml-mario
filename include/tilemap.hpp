#pragma once
#include "stdafx.h"
#include "window.hpp"

class TileMap: public sf::Drawable, public sf::Transformable
{
public:
    TileMap(std::string tilesheet, const int &tiles, const unsigned int height, const unsigned int width, const unsigned int tileSize);
    sf::FloatRect getTile(int pos);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool load();

private:
    bool initTexture(std::string& tilesheet) ;
    void draw(const Window& window);

private:
    unsigned int height;
    unsigned int width;
    unsigned int tile_size;
    sf::VertexArray vertices;
    const int* tiles;
    sf::Texture tileset;
};
