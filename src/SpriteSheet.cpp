#include "SpriteSheet.hpp"

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>

using json = nlohmann::json;

bool SpriteSheet::loadFromFile(const std::string& filename) {
    std::ifstream textureFile("C:/Users/grigo/Repos/sfml-framework/output.tex", std::ios::in | std::ios::binary);

    uint64_t imageSectionSize{};
    uint64_t metadataSectionSize{};
    textureFile.read(reinterpret_cast<char*>(&imageSectionSize), sizeof(imageSectionSize));
    textureFile.read(reinterpret_cast<char*>(&metadataSectionSize), sizeof(metadataSectionSize));

    std::unique_ptr<char[]> imageBuffer(new char[imageSectionSize]);
    std::unique_ptr<char[]> metadataBuffer(new char[metadataSectionSize]);
    textureFile.read(imageBuffer.get(), imageSectionSize);
    textureFile.read(metadataBuffer.get(), metadataSectionSize);

    m_texture = std::make_unique<sf::Texture>();
    if (!m_texture->loadFromMemory(imageBuffer.get(), imageSectionSize)) {
        std::cerr << "Error in loading texture from embedded image." << std::endl;
        return false;
    }

    json metadata = json::parse(metadataBuffer.get());
    for (const auto& elem : metadata.at("sprites")) {
        std::cout << elem.dump(2) << std::endl;
    }

    return true;
}

sf::Texture& SpriteSheet::getTexture() {
    return *m_texture;
}