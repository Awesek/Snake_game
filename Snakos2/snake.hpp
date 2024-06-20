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
    std::string getScoreString() const { return std::to_string(score); }
    std::string getLengthString() const { return std::to_string(snakeLength); }
    void increaseSpeed();
    void reduceScore(int points);
    int getScore() const { return score; }

    void update(float secondsElapsed);
    void draw(sf::RenderWindow* window);
    sf::Vector2f getPosition() const { return head.getPosition(); }
    sf::FloatRect getHeadBounds() const { return head.getGlobalBounds(); }

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
