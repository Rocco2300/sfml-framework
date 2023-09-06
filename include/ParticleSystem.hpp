#pragma once

#include "Particle.hpp"

class ParticleSystem : public sf::Transformable, public sf::Drawable {
private:
    std::vector<std::unique_ptr<Particle>> m_particles;

public:
    ParticleSystem() = default;

    template <typename P>
    void fuel(int particleNumber) {
        auto previousSize = m_particles.size();
        m_particles.resize(previousSize + particleNumber);
        std::generate_n(
                m_particles.begin() + previousSize, particleNumber, [&]() {
                    return std::unique_ptr<Particle>(new P(this));
                });
    }

    //    void setBounds(sf::FloatRect bounds);
    //    void setBoundStick(bool stickBounds);

    void update(sf::Time dt);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};