#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <memory>
#include <optional>

/**
 * @brief Tile is the representation of a single sprite.
 */
struct Tile {
    std::string name; // - name of the tile
    sf::Rect<uint16_t> bounds; // - tile bounds
};

/**
 * @brief Sector is a list of tiles that are related.
 */
struct Sector {
    std::string name; // - name of the sector
    sf::Vector2<uint16_t> origin; // - origin or the sector (top left)
    std::vector<Tile> tiles; // - list of tiles
};

/**
 * @brief SpriteSheet is a structure which contains and structures multiple
 * sprites into sectors of tiles (sprites)
 */
class SpriteSheet {
private:
    std::unique_ptr<sf::Texture> m_texture;
    std::vector<Sector> m_sectors;

public:
    /**
     * @brief Default constructor
     */
    SpriteSheet() = default;

    /**
     * @brief Load the texture data from file
     * @param filename - path to file
     * @return true on success, false otherwise
     */
    bool loadTextureFromFile(const std::string& filename);

    /**
     * @brief Load the metadata data from file
     * @param filename - path to file
     * @return true on success, false otherwise
     */
    bool loadMetadataFromFile(const std::string& filename);

    /**
     * @brief Get the underlying texture.
     * @return underlying sf::Texture
     */
    sf::Texture* getTexture();

    /**
     * @brief Check if a sector exists
     * @param name - name of the sector
     * @return true if found, false otherwise
     */
    bool containsSector(const std::string& name);

    /**
     * @brief Get the bounds of tile(sprite) by sector and index
     * @param name - name of sector
     * @param index - index of tile(sprite)
     * @return bounds if sprite exists, std::nullopt otherwise
     */
    std::optional<sf::IntRect> getTextureRect(const std::string& name,
                                              uint16_t index = 0);

private:
    Sector* getSector(const std::string& name);
    void loadMetadataFromBuffer(std::string_view metadata);
};