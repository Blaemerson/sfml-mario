#pragma once
#include "stdafx.h"

class TileMap: public sf::Drawable, public sf::Transformable
{
public:
    TileMap(const std::string& tilesheet, const int &tiles);
    bool initTexture(const std::string& tilesheet);
    bool load(sf::Vector2u tile_size, unsigned int width, unsigned int height);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    const int getTile(int pos);
private:

    sf::VertexArray m_vertices;
    const int* tiles;
    sf::Texture m_tileset;
};
