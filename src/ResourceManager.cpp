#include "ResourceManager.h"
ResourceManager* ResourceManager::m_instance = nullptr;

#include <iostream>

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
    auto beginning = path.rfind('/');
    auto fileName = path.substr(beginning + 1);

    auto dotPos = fileName.find('.');
    fileName = fileName.substr(0, dotPos);

    m_textures.load(fileName, path);
}