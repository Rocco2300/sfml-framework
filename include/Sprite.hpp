#pragma once

#include "SpriteSheet.hpp"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexArray.hpp"

#include <array>

class Sprite : public sf::Drawable, public sf::Transformable {
private:
    sf::VertexArray m_vertices;

    uint16_t m_frame{};
    std::string m_sector{};
    SpriteSheet* m_spriteSheet{};

public:
    Sprite();

    void setFrame(uint16_t frame);
    void setSector(const std::string& sector);
    void setSpriteSheet(SpriteSheet& spriteSheet);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void updatePositions();
    void updateTexCoords();
};