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

struct ParticleState {
    sf::Vector2f velocity{};
    float rotationAmt{};
};

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
    std::cout << sprite.getLocalBounds().width << ' ' << sprite.getLocalBounds().height << '\n';

    Particle<sf::Sprite, ParticleState> particle(sprite, 1.f);
    ParticleSystem particleSystem(particle);

    auto spawnLambda = [](decltype(particle)& p) {
        auto& state = p.getState();
        auto& velocity = state.velocity;
        if (velocity == sf::Vector2f(0.f, 0.f)) {
            velocity = sf::Vector2f((rand() % 600 - 300) / 10.f,
                                    (rand() % 600 - 300) / 10.f);
        }

        auto& rotationAmt = state.rotationAmt;
        if (rotationAmt == 0.f) {
            rotationAmt = rand() % 720 - 360;
        }
    };

    auto updateLambda = [](decltype(particle)& p, float dt) {
        p.setPosition(p.getPosition() + p.getState().velocity * dt);
        p.setRotation(p.getRotation() + p.getState().rotationAmt * dt);
    };

    particleSystem.setPosition(200.f, 200.f);
    particleSystem.setParticle(particle);
    particleSystem.setSpawnFunction(std::move(spawnLambda));
    particleSystem.setUpdateFunction(std::move(updateLambda));
    particleSystem.fuel(20);

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