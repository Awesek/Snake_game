#pragma once

#include "SFML/Graphics.hpp"
#include <vector>

class Snake
{
public:
    enum DIRECTION { UP, DOWN, LEFT, RIGHT };

    Snake(const std::vector<sf::RectangleShape>& obstacles);
    ~Snake();

    void changeDirection(DIRECTION d);
    void grabbedItem();
    bool checkCollisionWithItself();
    bool checkCollisionWithEdges();
    std::string getScoreString() const { return std::to_string(score); }
    std::string getLengthString() const { return std::to_string(snakeLength); }
    void increaseSpeed(float factor = 1.0f, float duration = 0.0f);
    void reduceScore(int points);
    int getScore() const { return score; }
    void increaseScore(int points) { score += points; }

    void update(float secondsElapsed);
    void draw(sf::RenderWindow* window);
    sf::Vector2f getPosition() const { return head.getPosition(); }
    sf::FloatRect getHeadBounds() const { return head.getGlobalBounds(); }

private:
    sf::Sprite head;
    sf::Sprite bodyPart;

    std::vector<sf::Vector2f> snakeParts;
    const std::vector<sf::RectangleShape>& obstacles;

    DIRECTION direction;
    int score;
    float timeAcummulator;
    float step;
    int snakeLength;

    float speedBoostFactor;
    float speedBoostDuration;
    sf::Clock speedBoostClock;
};
