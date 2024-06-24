#include "powerup.hpp"
#include "game.hpp"

PowerUp::PowerUp(Type type, const std::vector<sf::RectangleShape>& obstacles, const std::string& imagePath) : type(type), obstacles(obstacles)
{
    if (!texture.loadFromFile(imagePath)) {
        // Handle error
    }
    sprite.setTexture(texture);

    sf::FloatRect spriteSize = sprite.getLocalBounds();
    sprite.setOrigin(spriteSize.width / 2, spriteSize.height / 2);
    sprite.setScale(70.f / spriteSize.width, 65.f / spriteSize.height);

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
