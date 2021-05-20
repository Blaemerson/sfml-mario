#include "../include/tilemap.h"

TileMap::TileMap()
{
}

TileMap::~TileMap()
{
}

void TileMap::render(sf::RenderTarget& target)
{
	for (std::vector<Tile*>& tiles : this->tiles)
	{
		for (Tile*& tile : tiles)
		{
			tile->render(target);
		}
	}
}

void TileMap::addTile(unsigned x, unsigned y)
{
}
