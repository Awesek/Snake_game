#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include <vector>

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
    bool inMenu;

    int currentLevel;

    std::vector<sf::RectangleShape> obstacles;
    std::vector<sf::RectangleShape> menuButtons;
    std::vector<sf::Text> menuButtonLabels;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Clock playClock;
    sf::Text playTimeText;
    bool gameStarted;

    void checkCollisions();
    void resetGame();
    void handleInput();
    void handleMenuInput();
    void drawMenu();
    void drawObstacles();
    void increaseDifficulty();
    void loadMap(int level);
    void showMapDescription(int level);

    sf::RectangleShape playButton;
    sf::Text playButtonLabel;
    sf::Text mapDescription;

    sf::Clock timeLimitClock;
    sf::Text timeLimitText;
    int timeLimit;
};
