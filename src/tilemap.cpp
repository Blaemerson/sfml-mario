#include "../include/tilemap.hpp"

TileMap::TileMap(std::string tilesheet, const int &tiles, const unsigned int width, const unsigned int height, const unsigned int tile_size) {
  initTexture(tilesheet);
  this->height = height;
  this->width = width;
  this->tiles = &tiles;
  this->tile_size = tile_size;
}

bool TileMap::initTexture(std::string& path) {
  // load the tileset texture
  if (!tileset.loadFromFile(path))
  {
    return false;
  }
  return true;
}

bool TileMap::Load()
{
  // resize the vertex array to fit the level size
  vertices.setPrimitiveType(sf::Quads);
  vertices.resize(width * height * 4);

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
      int tu = tile_num % (tileset.getSize().x / tile_size);
      int tv = tile_num / (tileset.getSize().x / tile_size);

      // get a pointer to the current tile's quad
      sf::Vertex* quad = &vertices[(i + j * width) * 4];

      // define its 4 corners
      quad[0].position = sf::Vector2f(i * tile_size, j * tile_size);
      quad[1].position = sf::Vector2f((i + 1) * tile_size, j * tile_size);
      quad[2].position = sf::Vector2f((i + 1) * tile_size, (j + 1) * tile_size);
      quad[3].position = sf::Vector2f(i * tile_size, (j + 1) * tile_size);

      // define its 4 texture coordinates
      quad[0].texCoords = sf::Vector2f(tu * tile_size, tv * tile_size);
      quad[1].texCoords = sf::Vector2f((tu + 1) * tile_size, tv * tile_size);
      quad[2].texCoords = sf::Vector2f((tu + 1) * tile_size, (tv + 1) * tile_size);
      quad[3].texCoords = sf::Vector2f(tu * tile_size, (tv + 1) * tile_size);
    }
  }

  std::cout << "done drawing" << std::endl;
  return true;
}

sf::FloatRect TileMap::GetTile(int pos) {
  sf::Vertex* quad = &vertices[(pos) * 4];
  sf::FloatRect tile = sf::FloatRect(quad[0].position.x * 3.0, quad[0].position.y * 3.0, 48, 48);
  return tile;
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  // apply the transform
  states.transform = getTransform();
  // states.transform.translate(0, target.getSize().y - (48 * 8));
  states.transform.scale(3, 3);


  // apply the tileset texture
  states.texture = &tileset;

  // draw the vertex array
  target.draw(vertices, states);
}
