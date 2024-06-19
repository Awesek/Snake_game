#pragma once

#include "SFML/Graphics.hpp"

class Item
{
public:
	Item();
	~Item();

	void update();
	void draw(sf::RenderWindow* window);
	sf::Vector2f getPosition() { return sprite.getPosition(); }
	void setPosition(sf::Vector2f p) { sprite.setPosition(p); }
private:
	sf::Sprite sprite;


};

