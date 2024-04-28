#include  <SFML/Graphics.hpp> // replace quotes with less than and greater than symbols

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Not Tetris");

    sf::Event event;

    while (window.isOpen()) {

        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed) {

                window.close();
            }
        }
    }

    return 0;
}