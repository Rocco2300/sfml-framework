#include "SpriteSheet.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>

using json = nlohmann::json;

bool SpriteSheet::loadFromFile(const std::string& filename) {
    std::unique_ptr<sf::Image> image = load(filename);
    if (!image) {
        return false;
    }

    m_texture = std::make_unique<sf::Texture>();
    if (!m_texture->loadFromImage(*image)) {
        std::cerr << "Error in loading texture from embedded image." << std::endl;
        return false;
    }

    return true;
}

sf::Texture* SpriteSheet::getTexture() {
    return m_texture.get();
}

sf::Texture* SpriteSheet::getTexture(const std::string& name) {
    if (name == m_name) {
        return m_texture.get();
    }

    for (const auto& child : m_children) {
        if (child.m_name == name) {
            return child.m_texture.get();
        }
    }

    return nullptr;
}

std::optional<sf::IntRect> SpriteSheet::at(const std::string& name, uint16_t index) {
    auto spriteSheet = getSubSpriteSheet(name);

    auto x = index % spriteSheet->m_horizontalTileNo;
    auto y = index / spriteSheet->m_horizontalTileNo;

    if (x >= spriteSheet->m_horizontalTileNo || y >= spriteSheet->m_verticalTileNo) {
        return std::nullopt;
    }

    auto tileWidth = spriteSheet->m_width / spriteSheet->m_horizontalTileNo;
    auto tileHeight = spriteSheet->m_height / spriteSheet->m_verticalTileNo;
    return sf::IntRect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
}

SpriteSheet* SpriteSheet::getSubSpriteSheet(const std::string& name) {
    if (name == m_name) {
        return this;
    }

    for (auto& child : m_children) {
        if (child.m_name == name) {
            return &child;
        }
    }

    return nullptr;
}


std::unique_ptr<sf::Image> SpriteSheet::load(const std::string& filename) {
    std::ifstream textureFile("C:/Users/grigo/Repos/sfml-framework/output.tex", std::ios::in | std::ios::binary);

    uint64_t imageSectionSize{};
    uint64_t metadataSectionSize{};
    textureFile.read(reinterpret_cast<char*>(&imageSectionSize), sizeof(imageSectionSize));
    textureFile.read(reinterpret_cast<char*>(&metadataSectionSize), sizeof(metadataSectionSize));

    std::vector<std::byte> imageBuffer(imageSectionSize);
    std::vector<std::byte> metadataBuffer(metadataSectionSize);
    textureFile.read(reinterpret_cast<char*>(imageBuffer.data()), imageSectionSize);
    textureFile.read(reinterpret_cast<char*>(metadataBuffer.data()), metadataSectionSize);

    std::unique_ptr<sf::Image> image(new sf::Image);
    if (!image->loadFromMemory(reinterpret_cast<char*>(imageBuffer.data()), imageSectionSize)) {
        std::cerr << "Error in loading texture from embedded image." << std::endl;
        return nullptr;
    }

    json metadata = json::parse(reinterpret_cast<char*>(metadataBuffer.data()));
    for (const auto& elem : metadata.at("sprites")) {
        SpriteSheet child{};
        child.m_name = elem.at("name");
        child.m_width = elem.at("width");
        child.m_height = elem.at("height");
        child.m_horizontalTileNo = elem.at("tilesHorizontal");
        child.m_verticalTileNo = elem.at("tilesVertical");
        auto originX = elem.at("originX");
        auto originY = elem.at("originY");

        child.m_texture = std::make_unique<sf::Texture>();
        sf::IntRect area(originX, originY, child.m_width, child.m_height);
        if (!child.m_texture->loadFromImage(*image, area)) {
            std::cerr << "Error loading sub image from spritesheet!" << std::endl;
            break;
        }

        m_children.push_back(std::move(child));
    }

    return image;
}