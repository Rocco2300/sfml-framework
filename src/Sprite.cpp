#include "Sprite.hpp"

#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

Sprite::Sprite() {
    m_vertices = sf::VertexArray(sf::TriangleStrip, 4);
}

void Sprite::setFrame(uint16_t frame) {
    m_frame = frame;
    updatePositions();
    updateTexCoords();
}

void Sprite::setSector(const std::string& sector) {
    m_sector = sector;
    updatePositions();
    updateTexCoords();
}

void Sprite::setSpriteSheet(SpriteSheet& spriteSheet) {
    m_spriteSheet = &spriteSheet;
    updatePositions();
    updateTexCoords();
}

void Sprite::updateTexCoords() {
    auto bounds = m_spriteSheet->getTextureRect(m_sector, m_frame);
    if (!bounds.has_value()) {
        return;
    }

    auto f = static_cast<sf::FloatRect>(*bounds);
    m_vertices[0].texCoords = sf::Vector2f(f.left, f.top);
    m_vertices[1].texCoords = sf::Vector2f(f.left, f.top + f.height);
    m_vertices[2].texCoords = sf::Vector2f(f.left + f.width, f.top);
    m_vertices[3].texCoords = sf::Vector2f(f.left + f.width, f.top + f.height);
}

void Sprite::updatePositions() {
    auto bounds = m_spriteSheet->getTextureRect(m_sector, m_frame);
    if (!bounds.has_value()) {
        return;
    }

    auto f = static_cast<sf::FloatRect>(*bounds);
    m_vertices[0].position = sf::Vector2f(0.f, 0.f);
    m_vertices[1].position = sf::Vector2f(0.f, f.height);
    m_vertices[2].position = sf::Vector2f(f.width, 0.f);
    m_vertices[3].position = sf::Vector2f(f.width, f.height);
}

void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = m_spriteSheet->getTexture();
    target.draw(m_vertices, states);
}