#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/System/Clock.hpp"

#include <memory>
#include <vector>
#include <iostream>

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
    std::vector<ParticlePtr> m_particlePool;

    std::function<GeneratorFunc> m_generator;
    std::function<UpdaterFunc> m_updater;
    std::function<DrawerFunc> m_drawer;

    bool m_emitter{};
    float m_accumulator{};
    float m_emissionRate{};

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

    void setEmitter(bool emitter) { m_emitter = emitter; }

    void setEmissionRate(float emissionRate) {
        m_emissionRate = emissionRate / 60.f;
    }

    void fuel(int particleNumber) {
        auto& particles = (m_emitter) ? m_particlePool : m_particles;
        auto previousSize = particles.size();

        particles.resize(previousSize + particleNumber);
        std::generate_n(particles.begin() + previousSize, particleNumber,
                        [&]() {
                            auto particle = std::make_unique<ParticleType>();
                            if (!m_emitter) {
                                m_generator(*particle);
                            }
                            return std::move(particle);
                        });
    }

    void update(sf::Time dt) {
        if (m_emitter) {
            emitParticles();
        }

        for (int i = m_particles.size() - 1; i >= 0; i--) {
            auto isAlive = m_updater(*m_particles[i], dt);

            if (!isAlive) {
                killParticle(i);
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

private:
    void killParticle(size_t index) {
        if (m_emitter) {
            m_particlePool.push_back(std::move(m_particles[index]));
        }
        m_particles.erase(m_particles.begin() + index);
    }

    void emitParticles() {
        if (m_particlePool.empty()) {
            return;
        }

        m_accumulator += m_emissionRate;
        while (m_accumulator >= 1.f && !m_particlePool.empty()) {
            m_particles.push_back(std::move(m_particlePool.front()));
            m_generator(*m_particles.back());
            m_particlePool.erase(m_particlePool.begin());
            m_accumulator -= 1.f;
        }
//        auto emittedParticles = (m_emissionRate <= m_particlePool.size())
//                                        ? m_emissionRate
//                                        : m_particlePool.size();
//        for (int i = emittedParticles - 1; i >= 0; i--) {
//            m_particles.push_back(std::move(m_particlePool[i]));
//            m_generator(*m_particles.back());
//            m_particlePool.erase(m_particlePool.begin() + i);
//        }
    }
};