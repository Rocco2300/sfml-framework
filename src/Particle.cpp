#include "Particle.hpp"

#include <random>

Particle::Particle(ParticleSystem* system) : m_system{system} {}

bool Particle::isAlive() { return m_alive; }

void Particle::setLifetime(float lifetime) { m_lifetime = lifetime; }

BasicParticle::BasicParticle(ParticleSystem* system) : Particle(system) {
    m_alive = true;
    randomize();
}

Particle* BasicParticle::clone(ParticleSystem* system) const {
    return new BasicParticle(*this);
}

void BasicParticle::setSpeed(float speed) { m_speed = speed; }

void BasicParticle::setDirection(sf::Vector2f direction) {
    m_direction = direction;
}

void BasicParticle::update(sf::Time dt) {
    if (!m_alive) {
        return;
    }

    m_velocity = m_direction * m_speed * dt.asSeconds();
    auto pos = getPosition();
    setPosition(pos + m_velocity);

    m_lifetime -= dt.asSeconds();
    if (m_lifetime <= 0.f) {
        m_alive = false;
    }
}

void BasicParticle::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
    states.transform *= getTransform();

    if (m_alive) {
        target.draw(sf::CircleShape(1.f), states);
    }
}

void BasicParticle::randomize() {
    m_speed = rand() % 50 + 50;
    m_direction.x = (rand() % 200 - 100) / 100.f;
    m_direction.y = (rand() % 200 - 100) / 100.f;

    auto len =
            sqrtf(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
    m_direction.x /= len;
    m_direction.y /= len;
}