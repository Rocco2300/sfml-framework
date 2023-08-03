#pragma once

#include "ResourceHolder.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <filesystem>

class ResourceManager {
private:
    static ResourceManager* m_instance;

    using TextureHolder = ResourceHolder<std::string, sf::Texture>;
    using SoundBufferHolder = ResourceHolder<std::string, sf::SoundBuffer>;

    TextureHolder m_textures;
    SoundBufferHolder m_soundBuffers;

public:
    static ResourceManager* getInstance();
    ~ResourceManager();

    sf::Texture& getTexture(const std::string& id);
    void loadTexture(const std::string& path);
    void loadTextureDirectory(const std::string& path);

private:
    ResourceManager() = default;

    std::string getFileName(const std::string& path);

    template <typename Res, typename ResHolder>
    Res& getResource(const std::string& id, ResHolder& resourceHolder) {
        return resourceHolder.get(id);
    }

    template <typename ResHolder>
    void loadResource(const std::string& path, ResHolder& resourceHolder) {
        auto fileName = getFileName(path);
        resourceHolder.load(fileName, path);
    }

    template <typename ResHolder>
    void loadResourceDirectory(const std::string& path, ResHolder& resourceHolder) {
        namespace fs = std::filesystem;

        for (const auto& entry : fs::directory_iterator(path)) {
            auto pathString = entry.path().generic_string();
            auto fileName = getFileName(pathString);
            resourceHolder.load(fileName, pathString);
        }
    }
};