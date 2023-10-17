#include "Animator.hpp"

void Animator::setSprite(sf::Sprite& sprite) { m_sprite = &sprite; }

void Animator::setSpriteSheet(SpriteSheet& spriteSheet) {
    m_spriteSheet = &spriteSheet;

    if (!m_currentAnimation.empty() && m_sprite) {
        m_sprite->setTextureRect(*m_spriteSheet->getTextureRect(
                m_currentAnimation, m_currentFrame));
    }
}

void Animator::setCurrentAnimation(std::string_view currentAnimation) {
    m_currentFrame = 0;
    m_currentAnimation = currentAnimation;

    if (m_sprite && m_spriteSheet) {
        m_sprite->setTextureRect(*m_spriteSheet->getTextureRect(
                m_currentAnimation, m_currentFrame));
    }
}

void Animator::setFrameTimes(
        std::unordered_map<std::string, std::vector<float>>& frameTimes) {
    m_frameTimes = frameTimes;
}

void Animator::update(sf::Time dt) {
    m_accumulator += dt.asSeconds();

    auto& frameTimes = m_frameTimes[m_currentAnimation];
    if (m_accumulator >= frameTimes[m_currentFrame]) {
        m_accumulator = 0.f;
        m_currentFrame = (m_currentFrame + 1) % frameTimes.size();
        m_sprite->setTextureRect(*m_spriteSheet->getTextureRect(
                m_currentAnimation, m_currentFrame));
    }
}