#include "EventBroker.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"
#include "ResourceHolder.hpp"
#include "SFMLResourceLoader.hpp"
#include "SpriteSheet.hpp"
#include "SpriteSheetLoader.hpp"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;

const std::string Path = "C:/Users/grigo/Repos/sfml-framework/";

int main() {
    srand(time(nullptr));
    sf::err().rdbuf(nullptr);
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
    window.setFramerateLimit(60);

    ResourceHolder<std::string, sf::Texture> textureHolder;
    SFMLResourceLoader sfmlLoader(textureHolder, Path, ".png");

    ResourceHolder<std::string, SpriteSheet> spritesheetHolder;
    SpriteSheetLoader spriteSheetLoader(spritesheetHolder, Path, ".png", Path,
                                        ".json");

    spriteSheetLoader.load("player", "testing");

    sf::Sprite sprite;
    auto* spriteSheet = &spritesheetHolder.get("player");
    sprite.setTexture(*spriteSheet->getTexture());
    sprite.setTextureRect(*spriteSheet->getTextureRect("runningLeft", 3));
    sprite.setOrigin(24.f, 24.f);
    sprite.setScale({4.f, 4.f});
    std::cout << sprite.getLocalBounds().width << ' '
              << sprite.getLocalBounds().height << '\n';


    ParticleSystem particleSystem;
    particleSystem.setPosition(200.f, 200.f);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        particleSystem.fuel<BasicParticle>(10);
        particleSystem.update(dt);

        window.clear();
        window.draw(particleSystem);
        window.display();
    }

    return 0;
}