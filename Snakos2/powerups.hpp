#pragma once

#include "SFML/Graphics.hpp"
#include <vector>

class PowerUp
{
public:
    enum Type { SPEED_BOOST, EXTRA_POINTS };

    PowerUp(Type type, const std::vector<sf::RectangleShape>& obstacles);
    ~PowerUp();

    void update();
    void draw(sf::RenderWindow* window);
    sf::Vector2f getPosition() { return sprite.getPosition(); }
    void setPosition(sf::Vector2f p) { sprite.setPosition(p); }

private:
    sf::Sprite sprite;
    Type type;
    const std::vector<sf::RectangleShape>& obstacles;
};
