#include "ResourceHolder.hpp"
#include "SFMLResourceLoader.hpp"
#include "SpriteSheet.hpp"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;

int main() {
    sf::err().rdbuf(nullptr);
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    ResourceHolder<std::string, sf::Texture> resourceHolder;
    SFMLResourceLoader resourceLoader(resourceHolder, "C:/Users/grigo/Repos/sfml-framework/", ".png");

    resourceLoader.load("player", "testing");

    //    SpriteSheet spriteSheet;
    //    spriteSheet.loadFromFile("C:/Users/grigo/Repos/sfml-framework/output.tex");
    //
    sf::Sprite sprite;
    sprite.setTexture(resourceHolder.get("player"));
    sprite.setTextureRect(sf::IntRect(0, 0, 48, 48));
    sprite.setScale(4.f, 4.f);


    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}