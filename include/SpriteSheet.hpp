#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <optional>

struct Tile {
    std::string name;
    sf::Rect<uint16_t> bounds;
};

struct Sector {
    std::string name;
    sf::Vector2<uint16_t> origin;
    std::vector<Tile> tiles;
};

class SpriteSheet {
private:
    std::unique_ptr<sf::Texture> m_texture;
    std::vector<Sector> m_sectors;

public:
    SpriteSheet() = default;

    bool loadTextureFromFile(const std::string& filename);
    bool loadMetadataFromFile(const std::string& filename);

    sf::Texture* getTexture();
    bool containsSector(const std::string& name);
    std::optional<sf::IntRect> getTextureRect(const std::string& name,
                                              uint16_t index);

private:
    Sector* getSector(const std::string& name);
    void loadMetadataFromBuffer(std::string_view metadata);
};