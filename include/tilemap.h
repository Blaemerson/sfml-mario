#pragma once
#include "stdafx.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:

    bool load(const std::string& tileset, sf::Vector2u tile_size, const int* tiles, unsigned int width, unsigned int height);
    virtual void draw(sf::RenderTarget& target) const;
private:

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};
