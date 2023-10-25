#pragma once

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/System/Clock.hpp"

#include <iostream>
#include <memory>
#include <vector>

// TODO: somehow pass framerate to ParticleSystem
// TODO: might need a burst emitter, emit x particles at once every n frames

/**
 * @brief Basic particle system for custom particle types.
 * It operates on structs using function pointers which you
 * have to pass in.
 *
 * @tparam ParticleType - custom particle struct
 */
template <typename ParticleType>
class ParticleSystem : public sf::Transformable, public sf::Drawable {
public:
    using ParticlePtr = std::unique_ptr<ParticleType>;

    /**
     * @brief The initialization function of a particle
     * @param ParticleType& - the particle to be initialized
     */
    using GeneratorFunc = void(ParticleType&);
    /**
     * @brief The update function of a particle
     * @param ParticleType& - the particle to be updated
     * @param sf::Time - delta time
     * @return life state of particle after update
     */
    using UpdaterFunc = bool(ParticleType&, sf::Time);
    /**
     * @brief The draw function of a particle
     * @param ParticleType& - the particle to be drawn
     * @param sf::RenderTarget& - target to draw to
     * @param sf::RenderStates - passed states
     */
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
    /**
     * @brief Default constructor
     */
    ParticleSystem() = default;

    /**
     * @brief Setter for the initialization function of particles
     * @param generatorFunc - initialization function of particle
     */
    void setGenerator(std::function<GeneratorFunc> generatorFunc) {
        m_generator = generatorFunc;
    }

    /**
     * @brief Setter for the update function of the particles
     * @param updaterFunc - update function of particle
     */
    void setUpdater(std::function<UpdaterFunc> updaterFunc) {
        m_updater = updaterFunc;
    }

    /**
     * @brief Setter for the drawing function of particles
     * @param drawerFunc - draw function of particle
     */
    // clang-format off
    void setDrawer(std::function<DrawerFunc> drawerFunc) {
        m_drawer = drawerFunc;
    }
    // clang-format on

    /**
     * @brief Setter for the isEmitter property. If set to true,
     * system will emit n particles per second.
     *
     * @see setEmissionRate
     *
     * @param isEmitter - emission switch
     */
    void setEmitter(bool isEmitter) { m_emitter = isEmitter; }

    /**
     * @brief Setter for the emission rate of the system. The
     * emission rate has to be > 0.f. It is calculated as particles / second.
     *
     * @code
     * particleSystem.setEmissionRate(1.f); // One particle per second
     * particleSystem.setEmissionRate(1.f/3.f); // One particle every three seconds
     * @endcode
     *
     * @param emissionRate - emission rate in particles per second
     */
    void setEmissionRate(float emissionRate) {
        if (emissionRate < 0.f) {
            emissionRate = 1.f;
        }

        // TODO: make use of framerate instead of hardcoded 60.f
        m_emissionRate = emissionRate / 60.f;
    }

    /**
     * @brief Fuel the system with n particles. If it's an emitter
     * system they will be added in a particlePool, else they will be added in
     * the world.
     *
     * @code
     * // Normal
     * particleSystem.fuel(100);
     *
     * // Burst 10, then emit 1 per second
     * particleSystem.fuel(10);
     * particleSystem.setEmitter(true);
     * particleSystem.setEmissionRate(1.f);
     * @endcode
     *
     * @see setEmitter
     * @see setEmissionRate
     *
     * @param particleNumber - how many particles to insert
     */
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

    /**
     * @brief Update loop of the particle system. Call this every update cycle.
     * @param dt - delta time
     */
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

    /**
     * @brief Draw function of the particle system. Call every draw cycle.
     * @param target - target to draw to
     * @param states - passed states
     */
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
    }
};