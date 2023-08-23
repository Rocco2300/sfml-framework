#include "SpriteSheet.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <ranges>

namespace fs = std::filesystem;
using json = nlohmann::json;

sf::Texture* SpriteSheet::getTexture() {
    return m_texture.get();
}

bool SpriteSheet::containsSector(const std::string& name) {
    auto sameName = [&](const auto& sector) { return sector.name == name; };
    if (std::ranges::any_of(m_sectors, sameName)) {
        return true;
    }

    return false;
}

std::optional<sf::IntRect> SpriteSheet::getTextureRect(const std::string& name, uint16_t index) {
    auto sector = getSector(name);
    if (index < 0 || index > sector->tiles.size()) {
        return std::nullopt;
    }

    auto& bounds = sector->tiles[index].bounds;
    return sf::IntRect(bounds.left, bounds.top, bounds.width, bounds.height);
}

Sector* SpriteSheet::getSector(const std::string& name) {
    for (auto& sector: m_sectors) {
        if (sector.name == name) {
            return &sector;
        }
    }

    return nullptr;
}

void SpriteSheet::loadMetadataFromBuffer(std::string_view metadata) {
    json jsonMetadata = json::parse(metadata);
    for (const auto& elem: jsonMetadata.at("sectors")) {
        json pos = elem.at("pos");
        json tileNo = elem.at("tileNo");

        Sector sector{};
        sector.name = elem.at("name");
        sector.origin.x = pos.at("x");
        sector.origin.y = pos.at("y");

        uint8_t tileNoX = tileNo.at("x");
        uint8_t tileNoY = tileNo.at("y");
        uint16_t tileWidth = static_cast<int>(pos.at("w")) / tileNoX;
        uint16_t tileHeight = static_cast<int>(pos.at("h")) / tileNoY;

        for (int y = 0; y < tileNoY; y++) {
            for (int x = 0; x < tileNoX; x++) {
                Tile tile;
                tile.bounds.left = sector.origin.x + x * tileWidth;
                tile.bounds.top = sector.origin.y + y * tileHeight;
                tile.bounds.width = tileWidth;
                tile.bounds.height = tileHeight;
                sector.tiles.push_back(tile);
            }
        }

        m_sectors.push_back(sector);
    }
}

bool SpriteSheet::loadTextureFromFile(const std::string& filename) {
    std::ifstream textureFile(filename, std::ios::in | std::ios::binary);

    auto imageFileSize = fs::file_size(filename);
    std::vector<std::byte> imageBuffer(imageFileSize);
    textureFile.read(reinterpret_cast<char*>(imageBuffer.data()), imageFileSize);

    m_texture = std::make_unique<sf::Texture>();
    if (!m_texture->loadFromMemory(imageBuffer.data(), imageBuffer.size())) {
        std::cerr << "Error in loading texture " << filename << ".\n";
        return false;
    }

    return true;
}

bool SpriteSheet::loadMetadataFromFile(const std::string& filename) {
    std::ifstream metadataFile(filename);
    if (metadataFile.fail()) {
        return false;
    }

    std::stringstream metadataBuffer;
    metadataBuffer << metadataFile.rdbuf();

    // TODO: add error checking stuff?
    loadMetadataFromBuffer(metadataBuffer.str());
    return true;
}