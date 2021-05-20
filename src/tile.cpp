#include "../include/tile.h"


Tile::Tile(sf::Texture& texture_sheet, sf::IntRect texture_rect, bool damaging)
	:damaging(damaging)
{
	this->sprite.setTexture(texture_sheet);
	this->sprite.setTextureRect(texture_rect);
}

const sf::FloatRect Tile::getGlobalBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Tile::update()
{
}

void Tile::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

void Tile::initTexture()
{
	if (!this->texture_sheet.loadFromFile("textures/tile.png"))
	{
		std::cout << "ERROR in Tile: texturesheet failed to load" << std::endl;
	}
}

void Tile::initSprite()
{
	this->sprite.setTexture(this->texture_sheet);
	this->current_tile = sf::IntRect(0, 0, 16, 16);
	this->sprite.setTextureRect(this->current_tile);
	this->sprite.setScale(3.0f, 3.0f);
}
