#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>

template <typename DrawableType, typename StateType>
class Particle : public sf::Transformable, public sf::Drawable {
    static_assert(std::is_copy_constructible<DrawableType>::value);

private:
    DrawableType m_sprite;
    StateType m_state;

    bool m_alive{};
    float m_lifetime{1.f};

public:
    Particle() = default;
    Particle(DrawableType sprite, float lifetime = 1.f)
        : m_sprite{sprite}, m_alive{true}, m_lifetime{lifetime} {}

    Particle clone() { return *this; }
    StateType& getState() { return m_state; }
    const StateType& getState() const { return m_state; }
    bool isAlive() { return m_alive; }

    void setSprite(DrawableType sprite) { m_sprite = sprite; }
    void setLifetime(float lifetime) { m_lifetime = lifetime; }

    void update(sf::Time dt) {
        if (m_lifetime > 0.f) {
            m_lifetime -= dt.asSeconds();
            m_alive = m_lifetime > 0.f;
        }
    }

    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override {
        states.transform *= getTransform();
        if (m_alive) {
            target.draw(m_sprite, states);
        }
    }
};

template <typename ParticleType>
class ParticleSystem : public sf::Transformable, public sf::Drawable {
private:
    using SpawnFunction = std::function<void(ParticleType&)>;
    using UpdateFunction = std::function<void(ParticleType&, float)>;

    ParticleType m_prototypeParticle;
    std::vector<ParticleType> m_particles;

    SpawnFunction m_spawnFunction;
    UpdateFunction m_updateFunction;

public:
    ParticleSystem() = default;
    explicit ParticleSystem(const sf::Vector2f& pos) { setPosition(pos); }

    void fuel(int particleNumber) {
        auto previousSize = m_particles.size();
        m_particles.resize(previousSize + particleNumber);

        std::fill(m_particles.begin() + previousSize, m_particles.end(),
                  m_prototypeParticle);
        std::for_each(m_particles.begin() + previousSize, m_particles.end(),
                      m_spawnFunction);
    }

    void setParticle(ParticleType particle) { m_prototypeParticle = particle; }

    void setSpawnFunction(SpawnFunction&& spawnFunction) {
        m_spawnFunction = std::move(spawnFunction);
    }

    void setUpdateFunction(UpdateFunction&& updateFunction) {
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