#include "ResourceHolder.hpp"
#include "SFMLResourceLoader.hpp"
#include "SpriteSheetLoader.hpp"
#include "SpriteSheet.hpp"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;

const std::string Path = "C:/Users/grigo/Repos/sfml-framework/";

int main() {
    sf::err().rdbuf(nullptr);
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

    ResourceHolder<std::string, sf::Texture> resourceHolder;
    SFMLResourceLoader resourceLoader(resourceHolder, Path, ".png");

    ResourceHolder<std::string, SpriteSheet> spritesheetHolder;
    SpriteSheetLoader spriteSheetLoader(spritesheetHolder, Path, ".png", Path, ".json");

    resourceLoader.load("player", "testing");
    spriteSheetLoader.load("player", "testing");

    sf::Sprite sprite;
    sprite.setTexture(*spritesheetHolder.get("player").getTexture());
    sprite.setTextureRect(*spritesheetHolder.get("player").getTextureRect("runningLeft", 3));
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