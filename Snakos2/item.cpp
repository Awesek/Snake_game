#include "item.hpp"
#include "game.hpp"

Item::Item()
{
	sprite.setTexture(Game::commonTexture);

	sf::Rect<int> rect(sf::Vector2i(Game::TILESIZE * 2, 0), sf::Vector2i(Game::TILESIZE, Game::TILESIZE));
	sf::FloatRect spriteSize = sprite.getLocalBounds();
	sprite.setOrigin(spriteSize.width / 2, spriteSize.height / 2);
	sprite.setScale(Game::SCALE, Game::SCALE);
	sprite.setTextureRect(rect);

	sprite.setPosition(Game::getRandomPosition());
}
void Item::update()
{
	// TO DO: CHECK COLLISION
	// and change position if collided
}
Item::~Item()
{
}

void Item::draw(sf::RenderWindow* window)
{
	window->draw(sprite);
}

