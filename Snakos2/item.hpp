#pragma once

#include "SFML/Graphics.hpp"
#include <vector>

class Item
{
public:
    Item(const std::vector<sf::RectangleShape>& obstacles);
    ~Item();

    void update();
    void draw(sf::RenderWindow* window);
    sf::Vector2f getPosition() { return sprite.getPosition(); }
    void setPosition(sf::Vector2f p) { sprite.setPosition(p); }

private:
    sf::Sprite sprite;
    const std::vector<sf::RectangleShape>& obstacles;
};
