#pragma once

#include "ResourceHolder.h"

#include <SFML/Graphics/Texture.hpp>

class ResourceManager {
private:
    static ResourceManager* m_instance;

    ResourceHolder<std::string, sf::Texture> m_textures;

public:
    static ResourceManager* getInstance();
    ~ResourceManager();

    sf::Texture& getTexture(const std::string& id);
    void loadTexture(const std::string& path);

private:
    ResourceManager() = default;
};