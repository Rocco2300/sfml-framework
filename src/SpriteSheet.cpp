#include "SpriteSheet.hpp"

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

sf::Texture& SpriteSheet::getTexture() {
    return *m_texture;
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

    std::cout << reinterpret_cast<char*>(metadataBuffer.data()) << std::endl;
    json metadata = json::parse(reinterpret_cast<char*>(metadataBuffer.data()));
    for (const auto& elem : metadata.at("sprites")) {
        std::cout << elem.dump(2) << std::endl;
    }

    return image;
}