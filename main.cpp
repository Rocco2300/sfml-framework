#include "ResourceHolder.h"
#include <SFML/Graphics.hpp>

int main()
{
    sf::err().rdbuf(nullptr);
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    ResourceHolder<std::string, sf::Texture> textureHolder;
    textureHolder.load("testing", "C:/Users/grigo/Repos/sfml-framework/testing.png");

    sf::Sprite sprite;
    sprite.setTexture(textureHolder.get("testing"));
    sprite.setTextureRect(sf::IntRect(48, 0, 48, 48));
    sprite.setScale(4.f, 4.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}