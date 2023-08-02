#include "ResourceManager.hpp"

#include <iostream>
#include <filesystem>

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

sf::Texture& ResourceManager::getTexture(const std::string &id) {
    return m_textures.get(id);
}

void ResourceManager::loadTexture(const std::string& path) {
    auto fileName = getFileName(path);
    m_textures.load(fileName, path);
}

void ResourceManager::loadTextureDirectory(const std::string &path) {
    for (const auto& entry : fs::directory_iterator(path)) {
        auto pathString = entry.path().generic_string();
        auto fileName = getFileName(pathString);
        m_textures.load(fileName, pathString);
    }
}

std::string ResourceManager::getFileName(const std::string& path) {
    auto beginning = path.rfind('/');
    auto fileName = path.substr(beginning + 1);

    auto dotPos = fileName.find('.');
    fileName = fileName.substr(0, dotPos);
    return fileName;
}