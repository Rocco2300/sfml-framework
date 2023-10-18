#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"

#include "SpriteSheet.hpp"

class Animator {
private:
    float m_accumulator{};
    sf::Sprite* m_sprite{};
    uint16_t m_currentFrame{};
    SpriteSheet* m_spriteSheet{};
    std::string m_currentAnimation{};

    std::unordered_map<std::string, std::vector<float>> m_frameTimes;

public:
    Animator() = default;

    void setSprite(sf::Sprite& sprite);
    void setSpriteSheet(SpriteSheet& spriteSheet);
    void setCurrentAnimation(std::string_view currentAnimation);

    bool loadFromFile(const std::string& filename);

    void update(sf::Time dt);
};