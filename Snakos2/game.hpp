#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

class Snake;
class Item;

class Game
{
public:
    static int TILESIZE;
    static float SCALE;
    static float GAMEWIDTH;
    static float GAMEHEIGHT;
    static sf::Texture commonTexture;

    Game();
    ~Game();

    void start();
    static sf::Vector2f getRandomPosition();

private:
    sf::RenderWindow* mainWindow;
    sf::Event currentEvent;
    sf::Clock deltaClock;
    sf::Time deltaTime;

    Snake* snake;
    Item* item;

    sf::Font font;
    sf::Text textOnScreen;
    sf::Text gameOverText;

    bool isPaused;
    bool isGameOver;

    void checkCollisions();
    void resetGame();
    void handleInput();
};
