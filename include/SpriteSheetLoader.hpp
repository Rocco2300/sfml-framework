#pragma once

#include "ResourceLoader.hpp"
#include "SpriteSheet.hpp"

/**
 * @brief Concept checking if passed resource holder is holding SpriteSheets
 * @tparam ResHolder
 */
template <typename ResHolder>
concept SpriteSheetHolder =
        std::same_as<typename ResHolder::ResourceType, SpriteSheet>;

/**
 * @brief Resource loader class specialized for spritesheets
 * @tparam ResHolder - spritesheet holder
 */
template <typename ResHolder>
    requires SpriteSheetHolder<ResHolder>
class SpriteSheetLoader : public ResourceLoader<ResHolder> {
    using Identifier = typename ResHolder::IdentifierType;
    using Resource = typename ResHolder::ResourceType;

private:
    bool m_useSingleDirectory;

    std::string m_textureDirectory{};
    std::string m_textureFiletype{};
    std::string m_metadataDirectory{};
    std::string m_metadataFiletype{};

public:
    /**
     * @brief Default constructor
     */
    SpriteSheetLoader() = default;

    /**
     * @brief Construct the loader with ResourceHolder reference
     * @param resourceHolder - SpriteSheet holder
     * @param textureDirectory - directory to load texture resources from
     * @param textureFiletype - texture resource file extension
     * @param metadataDirectory - directory to load spritesheet metadata from
     * @param metadataFiletype - metadata file extension
     */
    SpriteSheetLoader(ResHolder& resourceHolder,
                      std::string_view textureDirectory = "",
                      std::string_view metadataDirectory = "",
                      std::string_view textureFiletype = "png",
                      std::string_view metadataFiletype = "json")
        : ResourceLoader<ResHolder>(resourceHolder),
          m_textureDirectory{textureDirectory},
          m_textureFiletype{textureFiletype},
          m_metadataDirectory{metadataDirectory},
          m_metadataFiletype{metadataFiletype} {}

    /**
     * @brief Setter for the texture directory
     * @param directory - path to directory
     */
    void setTextureDirectory(std::string_view directory) {
        m_textureDirectory = directory;
    }

    /**
     * @brief Setter for the metadata directory
     * @param directory - path to directory
     */
    void setMetadataDirectory(std::string_view directory) {
        m_metadataDirectory = directory;
    }

    /**
     * @brief Setter for the texture file extension
     * @param fileType - file extension (e.g. "png")
     */
    void setTextureFiletype(std::string_view fileType) {
        m_textureFiletype = fileType;
    }

    /**
     * @brief Setter for the metadata file extension
     * @param fileType - file extension (e.g. "json")
     */
    void setMetadataFiletype(std::string_view fileType) {
        m_metadataFiletype = fileType;
    }

    /**
     * @brief Set use single directory, if it's true, metadata files will
     * be loaded from textureDirectory instead of metadataDirectory
     * @param useSingleDirectory - single directory loading switch
     */
    void useSingleDirectory(bool useSingleDirectory) {
        m_useSingleDirectory = useSingleDirectory;
    }

    /**
     * @brief Load texture from texDir/filename.texExtension and
     * metadata from metadataDir/filename.metadataExtension, hence metadata
     * and texture file must have the same name
     * @param id - id to load resource at
     * @param filename - name of the file to load
     */
    void load(Identifier id, std::string_view filename) override {
        std::unique_ptr<Resource> resource(new Resource);

        auto texturePath = m_textureDirectory + std::string(filename) + '.' +
                           m_textureFiletype;
        if (!resource->loadTextureFromFile(texturePath)) {
            std::cerr << "Error loading texture of spritesheet " << filename
                      << ".\n";
        }

        auto metadataDirectory = (m_useSingleDirectory) ? m_textureDirectory
                                                        : m_metadataDirectory;
        auto metadataPath = metadataDirectory + std::string(filename) + '.' +
                            m_metadataFiletype;
        if (!resource->loadMetadataFromFile(metadataPath)) {
            std::cerr << "Error loading metadata of spritesheet " << filename
                      << ".\n";
        }

        this->m_resourceHolder->insert(id, std::move(resource));
    }
};