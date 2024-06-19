#pragma once

#include "SFML/Graphics.hpp"

#include <vector>

class Snake
{
public:
	enum DIRECTION { UP, DOWN, LEFT, RIGHT };

	Snake();
	~Snake();

	void changeDirection(DIRECTION d);
	void grabbedItem();
	bool checkCollisionWithItself();
	bool checkCollisionWithEdges();
	std::string getScoreString() { return std::to_string(score); }
	std::string getLengthString() { return std::to_string(snakeLength); }

	void update(float secondsElapsed);
	void draw(sf::RenderWindow* window);

	sf::Vector2f getPosition() { return head.getPosition(); }
private:
	sf::Sprite head;
	sf::Sprite bodyPart;

	std::vector<sf::Vector2f> snakeParts;

	DIRECTION direction;
	int score;
	float timeAcummulator;
	float step;
	int snakeLength;
};

