#include "ParticleSystem.hpp"

//void ParticleSystem::setBounds(sf::FloatRect bounds) {
//    m_hasBounds = true;
//    m_bounds = bounds;
//}
//
//void ParticleSystem::setBoundStick(bool stickBounds) {
//    m_stickBounds = stickBounds;
//}

void ParticleSystem::update(sf::Time dt) {
    for (auto& particle: m_particles) { particle->update(dt); }
}

void ParticleSystem::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
    states.transform *= getTransform();
    for (const auto& particle: m_particles) { target.draw(*particle, states); }
}