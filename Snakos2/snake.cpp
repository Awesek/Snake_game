#include "snake.hpp"
#include "game.hpp"

Snake::Snake(const std::vector<sf::RectangleShape>& obstacles) : obstacles(obstacles), direction(UP), timeAcummulator(0.f), step(0.1f), snakeLength(4), score(0), speedBoostFactor(1.0f), speedBoostDuration(0.0f)
{
    sf::Vector2f initialPos = Game::getRandomPosition(obstacles);

    head.setTexture(Game::commonTexture);
    sf::Rect<int> rect(sf::Vector2i(0, 0), sf::Vector2i(Game::TILESIZE, Game::TILESIZE));
    sf::FloatRect spriteSize = head.getLocalBounds();
    head.setOrigin(spriteSize.width / 2, spriteSize.height / 2);
    head.setScale(Game::SCALE, Game::SCALE);
    head.setTextureRect(rect);
    head.setPosition(initialPos);

    bodyPart.setTexture(Game::commonTexture);
    rect = sf::Rect<int>(sf::Vector2i(Game::TILESIZE, 0), sf::Vector2i(Game::TILESIZE, Game::TILESIZE));
    spriteSize = bodyPart.getLocalBounds();
    bodyPart.setOrigin(spriteSize.width / 2, spriteSize.height / 2);
    bodyPart.setScale(Game::SCALE, Game::SCALE);
    bodyPart.setTextureRect(rect);

    for (int i = 0; i <= snakeLength; i++)
        snakeParts.emplace_back(initialPos.x, initialPos.y + static_cast<float>(i * Game::SCALE * Game::TILESIZE));
}

Snake::~Snake()
{
}

void Snake::changeDirection(DIRECTION d)
{
    if ((direction == UP && d == DOWN) || (direction == DOWN && d == UP) ||
        (direction == LEFT && d == RIGHT) || (direction == RIGHT && d == LEFT))
        return;

    direction = d;
}

void Snake::grabbedItem()
{
    score += 1;
    sf::Vector2f pos = snakeParts[snakeLength];

    for (int i = snakeLength + 1; i <= snakeLength + 3; i++)
        snakeParts.emplace_back(pos);

    snakeLength += 3;
}

bool Snake::checkCollisionWithItself()
{
    float finalSize = (Game::TILESIZE * Game::SCALE) / 2.f;
    sf::Vector2f snakeHeadPos = snakeParts[0];
    sf::Vector2f partPos;

    for (int i = 1; i < snakeParts.size(); i++) {
        partPos = snakeParts[i];
        if (std::abs(partPos.x - snakeHeadPos.x) < finalSize && std::abs(partPos.y - snakeHeadPos.y) < finalSize)
            return true;
    }
    return false;
}

bool Snake::checkCollisionWithEdges()
{
    sf::Vector2f snakeHeadPos = snakeParts[0];

    if (snakeHeadPos.x < 0 || snakeHeadPos.x > Game::GAMEWIDTH || snakeHeadPos.y < 0 || snakeHeadPos.y > Game::GAMEHEIGHT)
        return true;
    return false;
}

void Snake::increaseSpeed(float factor, float duration)
{
    step *= factor;
    speedBoostFactor = factor;
    speedBoostDuration = duration;
    if (duration > 0.0f)
        speedBoostClock.restart();
}

void Snake::reduceScore(int points)
{
    score -= points;
    int partsToRemove = points * 3;
    for (int i = 0; i < partsToRemove && !snakeParts.empty(); ++i) {
        snakeParts.pop_back();
        --snakeLength;
    }
}

void Snake::update(float secondsElapsed)
{
    if (speedBoostDuration > 0.0f && speedBoostClock.getElapsedTime().asSeconds() > speedBoostDuration) {
        step /= speedBoostFactor;
        speedBoostFactor = 1.0f;
        speedBoostDuration = 0.0f;
    }

    timeAcummulator += secondsElapsed;
    if (timeAcummulator < step)
        return;
    timeAcummulator = 0;

    for (int i = snakeParts.size() - 1; i > 0; i--)
        snakeParts[i] = snakeParts[i - 1];

    sf::Vector2f currentPos = snakeParts[0];

    switch (direction)
    {
    case UP: currentPos.y -= Game::SCALE * Game::TILESIZE; break;
    case DOWN: currentPos.y += Game::SCALE * Game::TILESIZE; break;
    case LEFT: currentPos.x -= Game::SCALE * Game::TILESIZE; break;
    case RIGHT: currentPos.x += Game::SCALE * Game::TILESIZE; break;
    default: break;
    }

    snakeParts[0] = currentPos;
}

void Snake::draw(sf::RenderWindow* window)
{
    head.setPosition(snakeParts[0]);
    window->draw(head);

    for (int i = 1; i < snakeParts.size(); i++) {
        bodyPart.setPosition(snakeParts[i]);
        window->draw(bodyPart);
    }
}
