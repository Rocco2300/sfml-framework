#include "ResourceHolder.hpp"
#include "SpriteSheet.hpp"
#include "SpriteSheetLoader.hpp"

#include <SFML/Graphics.hpp>

const std::string Path = "../../";

// TODO: use a sprite made by me to be able to upload
int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
    window.setFramerateLimit(60);

    // Create a spritesheet holder
    ResourceHolder<std::string, SpriteSheet> spriteSheetHolder;
    // Create a spritesheet loader with reference to holder
    // We are only passing one path since metadata and texture are in the
    // same location, and we are using the default "png" and "json" types.
    SpriteSheetLoader spriteSheetLoader(spriteSheetHolder, Path);
    // Set useSingleDirectory true since we are only setting texture path
    spriteSheetLoader.useSingleDirectory(true);

    // Load the spritesheet
    spriteSheetLoader.load("player", "testing");

    // Get the spritesheet from the holder
    SpriteSheet& spriteSheet = spriteSheetHolder.get("player");

    // Use the spritesheet to init a sprite
    sf::Sprite sprite;
    // Get the texture from spritesheet
    sprite.setTexture(*spriteSheet.getTexture());
    // Get the bounds of the sprite you want by sector name and frame id
    sprite.setTextureRect(*spriteSheet.getTextureRect("runningDown", 1));
    sprite.setScale(4.f, 4.f);
    sprite.setPosition(100.f, 130.f);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            window.clear();
            window.draw(sprite);
            window.display();
        }
    }

    // Clean the resources
    spriteSheetHolder.remove("player");

    return 0;
}