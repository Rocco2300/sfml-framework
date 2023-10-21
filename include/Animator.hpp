#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

#include "Sprite.hpp"

class Animator {
private:
    Sprite* m_sprite;

    float m_accumulator{};
    uint16_t m_currentFrame{};
    std::string m_currentAnimation{};

    std::unordered_map<std::string, std::vector<float>> m_frameTimes;

public:
    Animator() = default;

    void setSprite(Sprite& sprite);
    void setCurrentAnimation(std::string_view currentAnimation);

    bool loadFromFile(const std::string& filename);

    void update(sf::Time dt);
};