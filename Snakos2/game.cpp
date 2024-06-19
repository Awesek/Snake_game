#include "game.hpp"
#include "snake.hpp"
#include "item.hpp"

#include <iostream>

Game::Game() : currentEvent()
{
	srand((unsigned)time(NULL));

	mainWindow = new sf::RenderWindow();

	commonTexture.loadFromFile("images/texture.png");

	snake = new Snake();
	item = new Item();


	font.loadFromFile("images/Sansation.ttf");

	textOnScreen.setFont(font);
	textOnScreen.setCharacterSize(24);
	textOnScreen.setFillColor(sf::Color::Red);
	textOnScreen.setPosition(32, mainWindow->getSize().y / 2);
}

Game::~Game()
{
	delete snake;
	delete mainWindow;
}

void Game::start()
{
	mainWindow->create(sf::VideoMode(800, 600), "Nibbles", sf::Style::Titlebar);

	// Game Loop
	bool playing = true;
	while (playing) {
		deltaTime = deltaClock.restart();

		while (mainWindow->pollEvent(currentEvent))
		{
			if (currentEvent.type == sf::Event::EventType::KeyPressed) {
				if (currentEvent.key.code == sf::Keyboard::Escape)
					playing = false;
				else if (currentEvent.key.code == sf::Keyboard::Up)
					snake->changeDirection(Snake::UP);
				else if (currentEvent.key.code == sf::Keyboard::Down)
					snake->changeDirection(Snake::DOWN);
				else if (currentEvent.key.code == sf::Keyboard::Left)
					snake->changeDirection(Snake::LEFT);
				else if (currentEvent.key.code == sf::Keyboard::Right)
					snake->changeDirection(Snake::RIGHT);
			}
		}

		// update all
		snake->update(deltaTime.asSeconds());
		item->update();

		checkCollisions();

		if (snake->checkCollisionWithItself()) playing = false;
		if (snake->checkCollisionWithEdges()) playing = false;

		textOnScreen.setString("SCORE: " + snake->getScoreString() + "\t\t\tSNAKE LENGTH: " + snake->getLengthString());

		// draw all
		mainWindow->clear();
		snake->draw(mainWindow);
		item->draw(mainWindow);
		mainWindow->draw(textOnScreen);
		mainWindow->display();
	}

	mainWindow->close();
}

sf::Vector2f Game::getRandomPosition()
{
	int xPositions = GAMEWIDTH / (SCALE * TILESIZE);
	int yPositions = GAMEHEIGHT / (SCALE * TILESIZE);

	int x = rand() % xPositions;
	int y = rand() % yPositions;

	return sf::Vector2f(x * (SCALE * TILESIZE), y * (SCALE * TILESIZE));
}

void Game::checkCollisions()
{
	float finalSize = (TILESIZE * SCALE) / 2.f;

	sf::Vector2f itemPos = item->getPosition();
	sf::Vector2f snakeHeadPos = snake->getPosition();

	// SNAKE HEAD WITH ITEM
	if (std::abs(itemPos.x - snakeHeadPos.x) < finalSize
		&& std::abs(itemPos.y - snakeHeadPos.y) < finalSize) {
		item->setPosition(getRandomPosition());
		snake->grabbedItem();
	}
}

int Game::TILESIZE = 32;
float Game::SCALE = 0.4f;
float Game::GAMEWIDTH = 800;
float Game::GAMEHEIGHT = 600;
sf::Texture Game::commonTexture;