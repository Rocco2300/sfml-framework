#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/System/Clock.hpp"

#include <memory>
#include <vector>

template <typename ParticleType>
class ParticleSystem : public sf::Transformable, public sf::Drawable {
public:
    using ParticlePtr = std::unique_ptr<ParticleType>;

    using GeneratorFunc = void(ParticleType&);
    using UpdaterFunc = bool(ParticleType&, sf::Time);
    using DrawerFunc = void(const ParticleType&,
                            sf::RenderTarget&,
                            sf::RenderStates);

private:
    std::vector<ParticlePtr> m_particles;

    std::function<GeneratorFunc> m_generator;
    std::function<UpdaterFunc> m_updater;
    std::function<DrawerFunc> m_drawer;

public:
    ParticleSystem() = default;

    void setGenerator(std::function<GeneratorFunc> function) {
        m_generator = function;
    }

    void setUpdater(std::function<UpdaterFunc> function) {
        m_updater = function;
    }

    // clang-format off
    void setDrawer(std::function<DrawerFunc> function) {
        m_drawer = function;
    }
    // clang-format on

    void fuel(int particleNumber) {
        auto previousSize = m_particles.size();
        m_particles.resize(previousSize + particleNumber);
        std::generate_n(m_particles.begin() + previousSize, particleNumber,
                        [&]() {
                            auto particle = std::make_unique<ParticleType>();
                            m_generator(*particle);
                            return std::move(particle);
                        });
    }

    void update(sf::Time dt) {
        for (int i = m_particles.size() - 1; i >= 0 ; i--) {
            auto isAlive = m_updater(*m_particles[i], dt);

            if (!isAlive) {
                m_particles.erase(m_particles.begin() + i);
            }
        }
    }

    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override {
        states.transform *= getTransform();
        for (const auto& particle: m_particles) {
            m_drawer(*particle, target, states);
        }
    }
};