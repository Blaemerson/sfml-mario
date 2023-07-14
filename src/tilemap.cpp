#include "../include/tilemap.h"

TileMap::TileMap(const std::string& tilesheet, const int &tiles, const unsigned int width, const unsigned int height) {
  initTexture(tilesheet);
  this->height = height;
  this->width = width;
  this->tiles = &tiles;
}

bool TileMap::initTexture(const std::string& path) {
  // load the tileset texture
  if (!m_tileset.loadFromFile(path))
  {
    return false;
  }
  return true;
}

bool TileMap::load(sf::Vector2u tile_size)
{
  // load the tileset texture
  // if (!m_tileset.loadFromFile(tileset))
  // {
  //   return false;
  // }

  // resize the vertex array to fit the level size
  m_vertices.setPrimitiveType(sf::Quads);
  m_vertices.resize(width * height * 4);

  // populate the vertex array, with one quad per tile
  for (unsigned int i = 0; i < width; ++i)
  {
    for (unsigned int j = 0; j < height; ++j)
    {
      // get the current tile number
      int tile_num = this->tiles[i + j * width];
      if (tile_num == 0) {
        // don't draw empty tiles
        continue;
      }

      // find its position in the tileset texture
      int tu = tile_num % (m_tileset.getSize().x / tile_size.x);
      int tv = tile_num / (m_tileset.getSize().x / tile_size.x);

      // get a pointer to the current tile's quad
      sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

      // define its 4 corners
      quad[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
      quad[1].position = sf::Vector2f((i + 1) * tile_size.x, j * tile_size.y);
      quad[2].position = sf::Vector2f((i + 1) * tile_size.x, (j + 1) * tile_size.y);
      quad[3].position = sf::Vector2f(i * tile_size.x, (j + 1) * tile_size.y);

      // define its 4 texture coordinates
      quad[0].texCoords = sf::Vector2f(tu * tile_size.x, tv * tile_size.y);
      quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size.x, tv * tile_size.y);
      quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size.x, (tv + 1) * tile_size.y);
      quad[3].texCoords = sf::Vector2f(tu * tile_size.x, (tv + 1) * tile_size.y);
    }
  }

  std::cout << "done drawing" << std::endl;
  return true;
}

const sf::FloatRect TileMap::getTile(int pos) {
  sf::Vertex* quad = &m_vertices[(pos) * 4];
  return sf::FloatRect(quad[0].position.x * 1.5, quad[0].position.y * 1.5, 32, 32);
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  // apply the transform
  states.transform = getTransform();
  // states.transform.translate(0, target.getSize().y - (48 * 8));
  states.transform.scale(1.5, 1.5);


  // apply the tileset texture
  states.texture = &m_tileset;

  // draw the vertex array
  target.draw(m_vertices, states);
}
