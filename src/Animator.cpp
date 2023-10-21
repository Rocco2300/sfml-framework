#include "Animator.hpp"

#include "nlohmann/json.hpp"

#include <fstream>

using json = nlohmann::json;

void Animator::setSprite(Sprite& sprite) { m_sprite = &sprite; }

// TODO: might have to add a check if sector exists \
         and if animation exists in map
void Animator::setCurrentAnimation(std::string_view currentAnimation) {
    m_currentFrame = 0;
    m_currentAnimation = currentAnimation;

    if (m_sprite) {
        m_sprite->setFrame(m_currentFrame);
        m_sprite->setSector(m_currentAnimation);
    }
}

bool Animator::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (in.fail()) {
        return false;
    }

    auto fileData = json::parse(in);
    in.close();

    auto animations = fileData.at("animations");
    for (const auto& animation: animations) {
        auto name = animation.at("name");
        auto frameTimes = animation.at("frameTimes");

        m_frameTimes.insert(std::make_pair(name, frameTimes));
    }

    return true;
}

void Animator::update(sf::Time dt) {
    m_accumulator += dt.asSeconds();

    auto& frameTimes = m_frameTimes[m_currentAnimation];
    if (m_accumulator >= frameTimes[m_currentFrame]) {
        m_accumulator = 0.f;
        m_currentFrame = (m_currentFrame + 1) % frameTimes.size();
        m_sprite->setFrame(m_currentFrame);
    }
}