#include "SpriteSheet.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>
#include <ranges>

using json = nlohmann::json;

bool SpriteSheet::loadFromFile(const std::string& filename) {
    auto [imageBuffer, metadataBuffer] = loadDataFromFile(filename);

    m_texture = std::make_unique<sf::Texture>();
    if (!m_texture->loadFromMemory(imageBuffer.data(), imageBuffer.size())) {
        std::cerr << "Error in loading texture from embedded image." << std::endl;
        return false;
    }

    loadMetadataFromBuffer(metadataBuffer);
    return true;
}

sf::Texture* SpriteSheet::getTexture() {
    return m_texture.get();
}

sf::Texture* SpriteSheet::getTexture(const std::string& name) {
    return m_texture.get();
}

bool SpriteSheet::contains(const std::string& name) {
    auto sameName = [&](const auto& sector) { return sector.name == name; };
    if (std::ranges::any_of(m_sectors, sameName)) {
        return true;
    }

    return false;
}

std::optional<sf::IntRect> SpriteSheet::at(const std::string& name, uint16_t index) {
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

void SpriteSheet::loadMetadataFromBuffer(const ByteBuffer& metadata) {
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

std::pair<ByteBuffer, ByteBuffer> SpriteSheet::loadDataFromFile(const std::string& filename) {
    std::ifstream textureFile("C:/Users/grigo/Repos/sfml-framework/output.tex", std::ios::in | std::ios::binary);

    uint64_t imageSectionSize{};
    uint64_t metadataSectionSize{};
    textureFile.read(reinterpret_cast<char*>(&imageSectionSize), sizeof(imageSectionSize));
    textureFile.read(reinterpret_cast<char*>(&metadataSectionSize), sizeof(metadataSectionSize));

    ByteBuffer imageBuffer(imageSectionSize);
    ByteBuffer metadataBuffer(metadataSectionSize);
    textureFile.read(reinterpret_cast<char*>(imageBuffer.data()), imageSectionSize);
    textureFile.read(reinterpret_cast<char*>(metadataBuffer.data()), metadataSectionSize);

    return {imageBuffer, metadataBuffer};
}