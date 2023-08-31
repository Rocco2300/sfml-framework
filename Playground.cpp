#include "Particle.hpp"
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

    ResourceHolder<std::string, SpriteSheet> spritesheetHolder;
    SpriteSheetLoader spriteSheetLoader(spritesheetHolder, Path, ".png", Path,
                                        ".json");

    spriteSheetLoader.load("player", "testing");

    sf::Sprite sprite;
    auto* spriteSheet = &spritesheetHolder.get("player");
    sprite.setTexture(*spriteSheet->getTexture());
    sprite.setTextureRect(*spriteSheet->getTextureRect("runningLeft", 3));

    Particle<sf::Sprite> particle(sprite, 1.f);
    ParticleSystem<sf::Sprite> particleSystem;
    particleSystem.setPosition(200.f, 200.f);
    particleSystem.setParticle(particle);

    std::map<uintptr_t, sf::Vector2f> velMap;
    auto updateLambda = [&](Particle<sf::Sprite>& particle, float dt) {
        sf::Vector2f velocity;
        uintptr_t particleAddress = reinterpret_cast<uintptr_t>(&particle);
        if (velMap.count(particleAddress)) {
            velocity = velMap[particleAddress];
        } else {
            velocity = sf::Vector2f((rand() % 600 - 300) / 10.f, (rand() % 600 - 300) / 10.f);
            velMap.insert(std::make_pair(particleAddress, velocity));
        }
        particle.setPosition(particle.getPosition() + velocity * dt);
    };

    particleSystem.fuel(20);
    particleSystem.setUpdateFunction(std::move(updateLambda));

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        particleSystem.update(dt);

        window.clear();
        window.draw(particleSystem);
        //        window.draw(sprite);
        window.display();
    }

    return 0;
}