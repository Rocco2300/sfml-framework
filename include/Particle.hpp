#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

//struct Particle {
//    sf::Sprite
//    sf::Vector2f position{};
//
//    float lifetime{};
//};

template <typename DrawableType>
class Particle : public sf::Transformable, public sf::Drawable {
    static_assert(std::is_copy_constructible<DrawableType>::value);

private:
    DrawableType m_sprite;
    float m_lifetime{1.f};

public:
    Particle() = default;
    Particle(DrawableType sprite, float lifetime = 1.f)
        : m_sprite{sprite}, m_lifetime{lifetime} {}

    Particle clone() { return *this; }

    void setSprite(DrawableType sprite) { m_sprite = sprite; }
    void setLifetime(float lifetime) { m_lifetime = lifetime; }

    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override {
        states.transform *= getTransform();
        target.draw(m_sprite, states);
    }
};

template <typename DrawableType>
class ParticleSystem : public sf::Transformable, public sf::Drawable {
private:
    Particle<DrawableType> m_prototypeParticle;
    std::vector<Particle<DrawableType>> m_particles;
    std::function<void(Particle<DrawableType>&, float)> m_updateFunction;

public:
    ParticleSystem() = default;
    explicit ParticleSystem(const sf::Vector2f& pos) { setPosition(pos); }

    void fuel(int particleNumber) {
        auto previousSize = m_particles.size();
        m_particles.resize(previousSize + particleNumber);
        std::fill(m_particles.begin() + previousSize, m_particles.end(),
                  m_prototypeParticle);
    }

    void setParticle(Particle<DrawableType> particle) {
        m_prototypeParticle = particle;
    }

    void setUpdateFunction(std::function<void(Particle<DrawableType>&, float)>&&
                                   updateFunction) {
        m_updateFunction = std::move(updateFunction);
    }

    void update(sf::Time dt) {
        for (auto& particle: m_particles) {
            m_updateFunction(particle, dt.asSeconds());
        }
    }

    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override {
        states.transform *= getTransform();
        for (const auto& particle: m_particles) {
            target.draw(particle, states);
        }
    }
};