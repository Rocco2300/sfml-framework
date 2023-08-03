#include "ResourceManager.hpp"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

ResourceManager* ResourceManager::m_instance = nullptr;

ResourceManager* ResourceManager::getInstance() {
    if (m_instance == nullptr) {
        m_instance = new ResourceManager();
    }

    return m_instance;
}

ResourceManager::~ResourceManager() {
    delete m_instance;
}

sf::Texture& ResourceManager::getTexture(const std::string& id) {
    return getResource<sf::Texture, TextureHolder>(id, m_textures);
}

void ResourceManager::loadTexture(const std::string& path) {
    loadResource<TextureHolder>(path, m_textures);
}

void ResourceManager::loadTextureDirectory(const std::string& path) {
    loadResourceDirectory<TextureHolder>(path, m_textures);
}

std::string ResourceManager::getFileName(const std::string& path) {
    auto beginning = path.rfind('/');
    auto fileName = path.substr(beginning + 1);

    auto dotPos = fileName.find('.');
    fileName = fileName.substr(0, dotPos);
    return fileName;
}