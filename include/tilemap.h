#pragma once
#include "stdafx.h"

class TileMap: public sf::Drawable, public sf::Transformable
{
public:
    TileMap(const std::string& tilesheet, const int &tiles, const unsigned int height, const unsigned int width);
    bool initTexture(const std::string& tilesheet);
    bool load(sf::Vector2u tile_size);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    const sf::FloatRect getTile(int pos);
private:

    unsigned int height;
    unsigned int width;
    sf::VertexArray m_vertices;
    const int* tiles;
    sf::Texture m_tileset;
};
