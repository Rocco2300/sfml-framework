#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <concepts>
#include <memory>

class ParticleSystem;

class Particle : public sf::Transformable, public sf::Drawable {
private:
    ParticleSystem* m_system{};

protected:
    bool m_alive{true};
    float m_lifetime{1.f};

public:
    Particle(ParticleSystem* system);
    virtual Particle* clone(ParticleSystem* system) const = 0;

    bool isAlive();

    void setLifetime(float lifetime);

    virtual void update(sf::Time dt) = 0;
    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override = 0;
};

class BasicParticle : public Particle {
private:
    sf::Vector2f m_velocity{};
    sf::Vector2f m_direction{};
    float m_speed{};

public:
    BasicParticle(ParticleSystem* system);
    Particle* clone(ParticleSystem* system) const override;

    void setSpeed(float speed);
    void setDirection(sf::Vector2f direction);

    void update(sf::Time dt) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void randomize();
};