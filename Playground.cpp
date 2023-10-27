#include "Animator.hpp"
#include "EventBroker.hpp"
#include "ParticleSystem.hpp"
#include "ResourceHolder.hpp"
#include "SFMLResourceLoader.hpp"
#include "SpriteSheet.hpp"
#include "SpriteSheetLoader.hpp"
#include "Sprite.hpp"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include <iostream>

using json = nlohmann::json;

const std::string Path = "C:/Users/grigo/Repos/sfml-framework/";

struct Particle {
    sf::Sprite sprite;

    float speed;
    float lifetime;
    sf::Vector2f dir;
    sf::Vector2f pos;
};

int main() {
    srand(time(nullptr));
    sf::err().rdbuf(nullptr);
    sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
    window.setFramerateLimit(60);

    ResourceHolder<std::string, SpriteSheet> spritesheetHolder;
    SpriteSheetLoader spriteSheetLoader(spritesheetHolder, Path);
    spriteSheetLoader.useSingleDirectory(true);

    spriteSheetLoader.load("player", "testing");

    sf::Sprite sprite;
    auto* spriteSheet = &spritesheetHolder.get("player");
    sprite.setTexture(*spriteSheet->getTexture());
    sprite.setTextureRect(*spriteSheet->getTextureRect("runningLeft", 3));
    sprite.setScale(4.f, 4.f);
    sprite.setPosition(30.f, 30.f);

    Sprite sprite1;
    sprite1.setSpriteSheet(*spriteSheet);
    sprite1.setSector("runningDown");
    sprite1.setFrame(2);
    sprite1.setScale(4.f, 4.f);
    sprite1.setPosition(-50.f, -50.f);


    Animator animator;
    animator.setSprite(sprite1);
    animator.setCurrentAnimation("runningLeft");
    if (!animator.loadFromFile(Path + "animation.json")) {
        std::cerr << "Error loading file\n";
    }

    const auto generator = [&](Particle& particle) {
        particle.sprite.setTexture(*spriteSheet->getTexture());
        particle.sprite.setTextureRect(
                *spriteSheet->getTextureRect("runningLeft", 3));
        particle.sprite.setOrigin(24.f, 24.f);

        particle.pos = sf::Vector2f(0.f, 0.f);
        particle.speed = 50.f;
        particle.lifetime = 1.f;
        particle.dir.x = ((rand() % 200) - 100) / 100.f;
        particle.dir.y = ((rand() % 200) - 100) / 100.f;
    };

    const auto updater = [](Particle& particle, sf::Time dt) -> bool {
        particle.pos += particle.speed * particle.dir * dt.asSeconds();
        particle.lifetime -= dt.asSeconds();

        return (particle.lifetime >= 0.f);
    };

    const auto drawer = [](const Particle& particle, sf::RenderTarget& target,
                           sf::RenderStates states) {
        states.transform.translate(particle.pos);
        target.draw(particle.sprite, states);
    };

    ParticleSystem<Particle> particleSystem;
    particleSystem.setPosition(200.f, 200.f);
    particleSystem.setGenerator(generator);
    particleSystem.setUpdater(updater);
    particleSystem.setDrawer(drawer);
    particleSystem.setEmitter(true);
    particleSystem.setEmissionRate(1.f);
    particleSystem.fuel(100);

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::A) {
                    sprite1.setSector("runningLeft");
                }
                if (event.key.code == sf::Keyboard::S) {
                    sprite1.setSector("runningDown");
                }
                if (event.key.code == sf::Keyboard::D) {
                    sprite1.setSector("runningRight");
                }
                if (event.key.code == sf::Keyboard::W) {
                    sprite1.setSector("runningUp");
                }
            }
        }

        particleSystem.update(dt);
        animator.update(dt);

        window.clear();
        window.draw(particleSystem);
        window.draw(sprite);
        window.draw(sprite1);
        window.display();
    }

    return 0;
}