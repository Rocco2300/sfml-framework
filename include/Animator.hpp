#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

#include "Sprite.hpp"

/**
 * @brief Animator class which animates the given sprite
 * based on the frame times loaded from file.
 */
class Animator {
private:
    Sprite* m_sprite{};

    float m_accumulator{};
    uint16_t m_currentFrame{};
    std::string m_currentAnimation{};

    std::unordered_map<std::string, std::vector<float>> m_frameTimes;

public:
    /**
     * @brief Default constructor
     */
    Animator() = default;

    /**
     * @brief Setter for the sprite reference that will be animated
     * @param sprite - sprite reference
     */
    void setSprite(Sprite& sprite);

    /**
     * @brief Function to set current animation that will be played
     * in the next update cycle
     *
     * @param currentAnimation - animation name
     */
    void setCurrentAnimation(std::string_view currentAnimation);

    /**
     * @brief Load the animation data from json file
     * @param filename - path to the json file
     * @return true on load, error otherwise
     */
    bool loadFromFile(const std::string& filename);

    /**
     * @brief Update function
     * @param dt - delta time
     */
    void update(sf::Time dt);
};