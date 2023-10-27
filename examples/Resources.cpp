#include "ResourceHolder.hpp"
#include "SFMLResourceLoader.hpp"

#include <SFML/Graphics.hpp>

const std::string Path = "../../";

// TODO: use a sprite made by me to be able to upload
int main() {
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
    window.setFramerateLimit(60);

    // Create a resource holder for sf::Texture
    ResourceHolder<std::string, sf::Texture> textureHolder;
    // Initialize resource(sfml resource holder) loader with reference to holder
    // We can omit the file type since we are using png by default
    SFMLResourceLoader sfmlLoader(textureHolder, Path);

    // Load the resource through the loader
    sfmlLoader.load("player", "testing");

    // Use the texture loaded in sprite
    sf::Sprite sprite;
    sprite.setTexture(textureHolder.get("player"));
    sprite.setTextureRect(sf::IntRect(0.f, 0.f, 48.f, 48.f));
    sprite.setScale(4.f, 4.f);
    sprite.setPosition(100.f, 130.f);

    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    // Clean the resources
    textureHolder.remove("player");

    return 0;
}