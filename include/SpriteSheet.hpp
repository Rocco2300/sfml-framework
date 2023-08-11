#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <optional>

using ByteBuffer = std::vector<std::byte>;

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

    bool loadFromFile(const std::string& filename);
    sf::Texture* getTexture();
    sf::Texture* getTexture(const std::string& name);

    bool contains(const std::string& name);
    std::optional<sf::IntRect> at(const std::string& name, uint16_t index);

private:
    Sector* getSector(const std::string& name);
    void loadMetadataFromBuffer(const ByteBuffer& metadata);
    std::pair<ByteBuffer, ByteBuffer> loadDataFromFile(const std::string& filename);
};