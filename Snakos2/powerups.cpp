#include "powerups.hpp"
#include "game.hpp"

PowerUp::PowerUp(Type type, const std::vector<sf::RectangleShape>& obstacles) : type(type), obstacles(obstacles)
{
    sprite.setTexture(Game::commonTexture);

    sf::Rect<int> rect;
    if (type == SPEED_BOOST) {
        rect = sf::Rect<int>(sf::Vector2i(Game::TILESIZE * 3, 0), sf::Vector2i(Game::TILESIZE, Game::TILESIZE));
    }
    else if (type == EXTRA_POINTS) {
        rect = sf::Rect<int>(sf::Vector2i(Game::TILESIZE * 4, 0), sf::Vector2i(Game::TILESIZE, Game::TILESIZE));
    }

    sf::FloatRect spriteSize = sprite.getLocalBounds();
    sprite.setOrigin(spriteSize.width / 2, spriteSize.height / 2);
    sprite.setScale(Game::SCALE, Game::SCALE);
    sprite.setTextureRect(rect);

    sprite.setPosition(Game::getRandomPosition(obstacles));
}

PowerUp::~PowerUp()
{
}

void PowerUp::update()
{
    // Power-ups logic (if needed)
}

void PowerUp::draw(sf::RenderWindow* window)
{
    window->draw(sprite);
}
