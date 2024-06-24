#include "game.hpp"
#include "snake.hpp"
#include "item.hpp"
#include "powerup.hpp"

#include <iostream>
#include <cmath>

int Game::TILESIZE = 32;
float Game::SCALE = 0.4f;
float Game::GAMEWIDTH = 800;
float Game::GAMEHEIGHT = 600;
sf::Texture Game::commonTexture;

Game::Game() : currentEvent(), isPaused(false), isGameOver(false), inMenu(true), gameStarted(false), currentLevel(0), timeLimit(7)
{
    srand(static_cast<unsigned>(time(NULL)));

    mainWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Nibbles", sf::Style::Default);
    mainWindow->setVerticalSyncEnabled(true);

    commonTexture.loadFromFile("images/texture.png");

    backgroundTexture.loadFromFile("images/background.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(0, 0);

    snake = new Snake(obstacles);
    item = new Item(obstacles);
    speedBoost = new PowerUp(PowerUp::SPEED_BOOST, obstacles, "images/speed_boost.png");
    extraPoints = new PowerUp(PowerUp::EXTRA_POINTS, obstacles, "images/extra_points.png");

    font.loadFromFile("images/Sansation.ttf");

    textOnScreen.setFont(font);
    textOnScreen.setCharacterSize(24);
    textOnScreen.setFillColor(sf::Color::Red);
    textOnScreen.setPosition(32.f, static_cast<float>(mainWindow->getSize().y) / 2);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setString("Game Over\nPress R to Restart");
    gameOverText.setPosition(200.f, 250.f);

    playTimeText.setFont(font);
    playTimeText.setCharacterSize(24);
    playTimeText.setFillColor(sf::Color::White);
    playTimeText.setPosition(32.f, 32.f);

    timeLimitText.setFont(font);
    timeLimitText.setCharacterSize(24);
    timeLimitText.setFillColor(sf::Color::Red);
    timeLimitText.setPosition(600.f, 32.f);

    mapDescription.setFont(font);
    mapDescription.setCharacterSize(24);
    mapDescription.setFillColor(sf::Color::White);
    mapDescription.setPosition(50.f, 450.f);

    playButton.setSize(sf::Vector2f(200.f, 50.f));
    playButton.setPosition(300.f, 500.f);
    playButton.setFillColor(sf::Color::Green);

    playButtonLabel.setFont(font);
    playButtonLabel.setString("Play");
    playButtonLabel.setCharacterSize(24);
    playButtonLabel.setFillColor(sf::Color::White);
    playButtonLabel.setPosition(360.f, 510.f);

    // Create menu buttons
    for (int i = 0; i < 3; ++i) {
        sf::RectangleShape button(sf::Vector2f(200.f, 50.f));
        button.setPosition(300.f, 150.f + i * 70);
        button.setFillColor(sf::Color::Blue);
        menuButtons.push_back(button);

        sf::Text label;
        label.setFont(font);
        label.setString("Map " + std::to_string(i + 1));
        label.setCharacterSize(24);
        label.setFillColor(sf::Color::White);
        label.setPosition(340.f, 160.f + i * 70);
        menuButtonLabels.push_back(label);
    }

    menuButtonLabels[2].setString("Classic");

    // Pause text
    pauseText.setFont(font);
    pauseText.setCharacterSize(48);
    pauseText.setFillColor(sf::Color::White);
    pauseText.setString("PAUSE");
    pauseText.setPosition(300.f, 250.f);

    menuButton.setSize(sf::Vector2f(200.f, 50.f));
    menuButton.setPosition(300.f, 350.f);
    menuButton.setFillColor(sf::Color::Red);

    menuButtonLabel.setFont(font);
    menuButtonLabel.setString("MENU");
    menuButtonLabel.setCharacterSize(24);
    menuButtonLabel.setFillColor(sf::Color::White);
    menuButtonLabel.setPosition(360.f, 360.f);

    powerUpSpawnInterval = 7 + rand() % 9; // Interval between 7 to 15 seconds
}

Game::~Game()
{
    delete snake;
    delete item;
    delete speedBoost;
    delete extraPoints;
    delete mainWindow;
}

void Game::start()
{
    bool playing = true;
    while (playing) {
        deltaTime = deltaClock.restart();

        while (mainWindow->pollEvent(currentEvent))
        {
            if (currentEvent.type == sf::Event::Closed)
                playing = false;

            if (inMenu)
                handleMenuInput();
            else
                handleInput();
        }

        if (inMenu) {
            drawMenu();
        }
        else {
            if (!isPaused && !isGameOver) {
                if (!gameStarted) {
                    gameStarted = true;
                    playClock.restart();
                    timeLimitClock.restart();
                    powerUpSpawnClock.restart();
                }

                snake->update(deltaTime.asSeconds());
                item->update();
                speedBoost->update();
                extraPoints->update();

                checkCollisions();
                handlePowerUpCollisions();

                if (snake->checkCollisionWithItself() || snake->checkCollisionWithEdges())
                    isGameOver = true;

                textOnScreen.setString("SCORE: " + snake->getScoreString() + "\t\t\tSNAKE LENGTH: " + snake->getLengthString());
                textOnScreen.setPosition(0, 0);

                sf::Time playTime = playClock.getElapsedTime();
                playTimeText.setString("Time: " + std::to_string(static_cast<int>(playTime.asSeconds())) + "s");

                if (currentLevel == 2) {
                    sf::Time timeLimitTime = timeLimitClock.getElapsedTime();
                    int timeLeft = timeLimit - static_cast<int>(timeLimitTime.asSeconds());
                    timeLimitText.setString("Time Left: " + std::to_string(timeLeft) + "s");
                    if (timeLeft <= 0) {
                        snake->reduceScore(2);
                        timeLimitClock.restart();
                    }
                    if (snake->getScore() < 0)
                        isGameOver = true;
                }

                spawnPowerUps();
            }

            mainWindow->clear();
            mainWindow->draw(backgroundSprite);  // Draw the background first
            snake->draw(mainWindow);
            item->draw(mainWindow);
            speedBoost->draw(mainWindow);
            extraPoints->draw(mainWindow);
            drawObstacles();
            mainWindow->draw(textOnScreen);
            mainWindow->draw(playTimeText);
            if (currentLevel == 2)
                mainWindow->draw(timeLimitText);

            if (isPaused) {
                mainWindow->draw(pauseText);
                mainWindow->draw(menuButton);
                mainWindow->draw(menuButtonLabel);
            }

            if (isGameOver)
                mainWindow->draw(gameOverText);

            mainWindow->display();
        }
    }

    mainWindow->close();
}

void Game::handleInput()
{
    if (currentEvent.type == sf::Event::KeyPressed) {
        if (currentEvent.key.code == sf::Keyboard::Escape)
            isPaused = !isPaused;
        else if (currentEvent.key.code == sf::Keyboard::P)
            isPaused = !isPaused;
        else if (currentEvent.key.code == sf::Keyboard::Up)
            snake->changeDirection(Snake::UP);
        else if (currentEvent.key.code == sf::Keyboard::Down)
            snake->changeDirection(Snake::DOWN);
        else if (currentEvent.key.code == sf::Keyboard::Left)
            snake->changeDirection(Snake::LEFT);
        else if (currentEvent.key.code == sf::Keyboard::Right)
            snake->changeDirection(Snake::RIGHT);
        else if (currentEvent.key.code == sf::Keyboard::R && isGameOver) {
            playClock.restart();
            timeLimitClock.restart();
            resetGame();
        }
    }

    if (isPaused && currentEvent.type == sf::Event::MouseButtonPressed) {
        if (currentEvent.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*mainWindow);
            if (menuButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                isPaused = false;
                inMenu = true;
                resetGame();
            }
        }
    }
}

void Game::handleMenuInput()
{
    if (currentEvent.type == sf::Event::MouseButtonPressed) {
        if (currentEvent.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*mainWindow);
            if (playButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) && currentLevel != 0) {
                inMenu = false;
                loadMap(currentLevel);
            }
            else {
                for (size_t i = 0; i < menuButtons.size(); ++i) {
                    if (menuButtons[i].getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentLevel = static_cast<int>(i + 1);
                        showMapDescription(currentLevel);
                        break;
                    }
                }
            }
        }
    }
}

void Game::drawMenu()
{
    mainWindow->clear();
    for (auto& button : menuButtons)
        mainWindow->draw(button);
    for (auto& label : menuButtonLabels)
        mainWindow->draw(label);
    if (currentLevel != 0) {
        mainWindow->draw(playButton);
        mainWindow->draw(playButtonLabel);
        mainWindow->draw(mapDescription);
    }
    mainWindow->display();
}

void Game::showMapDescription(int level)
{
    switch (level) {
    case 1:
        mapDescription.setString("Map 1: Obstacles spawn after every apple you eat.");
        break;
    case 2:
        mapDescription.setString("Map 2: Map 1 + limited time to eat the next apple.");
        break;
    case 3:
        mapDescription.setString("Classic: No obstacles, just snake.");
        break;
    }
}

void Game::drawObstacles()
{
    for (auto& obstacle : obstacles)
        mainWindow->draw(obstacle);
}

void Game::increaseDifficulty()
{
    snake->increaseSpeed();
}

void Game::loadMap(int level)
{
    currentLevel = level;
    obstacles.clear();
    // Define obstacles for each level
    if (level == 1) {
        // Add initial obstacles
    }
    else if (level == 2) {
        // Add initial obstacles
    }

    // Start game
    isPaused = false;
    isGameOver = false;
    resetGame();
}

sf::Vector2f Game::getRandomPosition(const std::vector<sf::RectangleShape>& obstacles)
{
    int xPositions = static_cast<int>(GAMEWIDTH / (SCALE * TILESIZE));
    int yPositions = static_cast<int>(GAMEHEIGHT / (SCALE * TILESIZE));

    sf::Vector2f position;
    bool positionIsValid = false;

    while (!positionIsValid) {
        int x = rand() % xPositions;
        int y = rand() % yPositions;
        position = sf::Vector2f(static_cast<float>(x * (SCALE * TILESIZE)), static_cast<float>(y * (SCALE * TILESIZE)));

        positionIsValid = true;
        for (auto& obstacle : obstacles) {
            if (std::abs(position.x - obstacle.getPosition().x) < TILESIZE * SCALE && std::abs(position.y - obstacle.getPosition().y) < TILESIZE * SCALE) {
                positionIsValid = false;
                break;
            }
        }
    }

    return position;
}

void Game::checkCollisions()
{
    float finalSize = (TILESIZE * SCALE) / 2.f;

    sf::Vector2f itemPos = item->getPosition();
    sf::Vector2f snakeHeadPos = snake->getPosition();

    if (std::abs(itemPos.x - snakeHeadPos.x) < finalSize && std::abs(itemPos.y - snakeHeadPos.y) < finalSize) {
        item->setPosition(getRandomPosition(obstacles));
        snake->grabbedItem();

        // Handle map-specific behavior
        if (currentLevel == 1 || currentLevel == 2) {
            // Map 1 and 2: Spawn obstacle after every apple
            sf::RectangleShape newObstacle(sf::Vector2f(50.f, 50.f));
            newObstacle.setPosition(getRandomPosition(obstacles));
            newObstacle.setFillColor(sf::Color::Blue);
            obstacles.push_back(newObstacle);
        }

        if (currentLevel == 2) {
            // Reset the time limit timer for Map 2
            timeLimitClock.restart();
        }
    }

    for (auto& obstacle : obstacles) {
        if (obstacle.getGlobalBounds().intersects(snake->getHeadBounds())) {
            isGameOver = true;
            resetObstacles();
        }
    }
}

void Game::handlePowerUpCollisions()
{
    float hitboxWidth = 70.f * Game::SCALE;
    float hitboxHeight = 65.f * Game::SCALE;
    sf::Vector2f snakeHeadPos = snake->getPosition();

    sf::Vector2f speedBoostPos = speedBoost->getPosition();
    if (std::abs(speedBoostPos.x - snakeHeadPos.x) < hitboxWidth && std::abs(speedBoostPos.y - snakeHeadPos.y) < hitboxHeight) {
        snake->increaseSpeed(0.65f, 3.0f);  // Increase speed by 60% for 3 seconds
        speedBoost->setPosition(getRandomPosition(obstacles));
    }

    sf::Vector2f extraPointsPos = extraPoints->getPosition();
    if (std::abs(extraPointsPos.x - snakeHeadPos.x) < hitboxWidth && std::abs(extraPointsPos.y - snakeHeadPos.y) < hitboxHeight) {
        snake->grabExtraPoints();  // Increase score and length
        extraPoints->setPosition(getRandomPosition(obstacles));
    }
}

void Game::resetGame()
{
    delete snake;
    snake = new Snake(obstacles);
    item->setPosition(getRandomPosition(obstacles));
    speedBoost->setPosition(getRandomPosition(obstacles));
    extraPoints->setPosition(getRandomPosition(obstacles));
    playClock.restart();
    timeLimitClock.restart();
    powerUpSpawnClock.restart();
    powerUpSpawnInterval = 7 + rand() % 9;
    isGameOver = false;
    resetObstacles();
}

void Game::resetObstacles()
{
    obstacles.clear();
}

void Game::spawnPowerUps()
{
    sf::Time elapsed = powerUpSpawnClock.getElapsedTime();
    if (elapsed.asSeconds() > powerUpSpawnInterval) {
        speedBoost->setPosition(getRandomPosition(obstacles));
        extraPoints->setPosition(getRandomPosition(obstacles));
        powerUpSpawnClock.restart();
        powerUpSpawnInterval = 7 + rand() % 9; // Reset the interval
    }
}
