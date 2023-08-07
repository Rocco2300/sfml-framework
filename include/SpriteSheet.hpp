#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <memory>

class SpriteSheet {
private:
    std::unique_ptr<sf::Texture> m_texture;
    std::string m_name;
    uint16_t m_width;
    uint16_t m_height;
    uint8_t m_horizontalTileNo;
    uint8_t m_verticalTileNo;
    std::vector<SpriteSheet> m_children;

public:
    SpriteSheet() = default;

    bool loadFromFile(const std::string& filename);
    sf::Texture& getTexture();
};