#pragma once

#include "SpriteSheet.hpp"

#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/Transformable.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/VertexArray.hpp"

#include <array>

/**
 * @brief Custom sprite class for use with Animator
 * @see Animator
 */
class Sprite : public sf::Drawable, public sf::Transformable {
private:
    sf::VertexArray m_vertices;

    uint16_t m_frame{};
    std::string m_sector{};
    SpriteSheet* m_spriteSheet{};

public:
    /**
     * @brief Default constructor
     */
    Sprite();

    /**
     * @brief Setter for the current frame/tile
     * @param frame - sector frame/tile number
     */
    void setFrame(uint16_t frame);

    /**
     * @brief Setter for the current sector
     * @param sector - current sector name
     */
    void setSector(const std::string& sector);

    /**
     * @brief Setter for the spritesheet reference
     * @param spriteSheet - spritesheet reference (non-owning)
     */
    void setSpriteSheet(SpriteSheet& spriteSheet);

    /**
     * @brief Draw function for the sprite
     * @param target - target to draw to
     * @param states - passed states
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void updatePositions();
    void updateTexCoords();
};