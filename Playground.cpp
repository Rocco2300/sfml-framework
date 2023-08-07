#include "ResourceManager.hpp"
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

//    ResourceManager* resManager = ResourceManager::getInstance();
//    resManager->loadTexture("C:/Users/grigo/Repos/sfml-framework/testing.png");
//    resManager->loadTextureDirectory("C:/Users/grigo/Pictures/sprites");
//
//    sf::Sprite sprite;
//    sprite.setTexture(resManager->getTexture("testing"));
//    sprite.setTextureRect(sf::IntRect(48, 0, 48, 48));
//    sprite.setScale(4.f, 4.f);
    SpriteSheet spriteSheet;
    spriteSheet.loadFromFile("C:/Users/grigo/Repos/sfml-framework/output.tex");

    sf::Sprite sprite;
    sprite.setTexture(spriteSheet.getTexture());
    sprite.setTextureRect(sf::IntRect(48, 0, 48, 48));
    sprite.setScale(4.f, 4.f);

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