#include "SpriteSheet.hpp"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <memory>

using json = nlohmann::json;

int main() {
    sf::err().rdbuf(nullptr);
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    SpriteSheet spriteSheet;
    spriteSheet.loadFromFile("C:/Users/grigo/Repos/sfml-framework/output.tex");

    std::cout << "Spritesheet size: " << sizeof(SpriteSheet) << std::endl;

    sf::Sprite sprite;
    sprite.setTexture(*spriteSheet.getTexture());
    sprite.setTextureRect(*spriteSheet.getTextureRect("runningLeft", 3));
    sprite.setScale(4.f, 4.f);

    std::cout << "Sector size: " << sizeof(Sector) << '\n';
    std::cout << "Tile size: " << sizeof(Tile) << '\n';

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}