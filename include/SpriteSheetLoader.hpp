#pragma once

#include "ResourceLoader.hpp"
#include "SpriteSheet.hpp"

template <typename ResHolder>
class SpriteSheetLoader : public ResourceLoader<ResHolder> {

private:
    using Identifier = typename ResHolder::IdentifierType;
    using Resource = typename ResHolder::ResourceType;

    static_assert(std::is_same<Resource, SpriteSheet>::value,
                  "Error: SpriteSheet specialization required");

    std::string m_textureDirectory{};
    std::string m_textureFiletype{};
    std::string m_metadataDirectory{};
    std::string m_metadataFiletype{};

public:
    SpriteSheetLoader() = default;
    // TODO: fix this atrocity
    SpriteSheetLoader(ResHolder& resourceHolder,
                      std::string_view textureDirectory = "",
                      std::string_view textureFiletype = "",
                      std::string_view metadataDirectory = "",
                      std::string_view metadataFiletype = "")
        : ResourceLoader<ResHolder>(resourceHolder),
          m_textureDirectory{textureDirectory},
          m_textureFiletype{textureFiletype},
          m_metadataDirectory{metadataDirectory},
          m_metadataFiletype{metadataFiletype} {}

    void setTextureDirectory(std::string_view directory) {
        m_textureDirectory = directory;
    }
    void setMetadataDirectory(std::string_view directory) {
        m_metadataDirectory = directory;
    }

    void setTextureFiletype(std::string_view fileType) {
        m_textureFiletype = fileType;
    }
    void setMetadataFiletype(std::string_view fileType) {
        m_metadataFiletype = fileType;
    }


    void load(Identifier id, std::string_view filename) override {
        // to be done
        std::unique_ptr<Resource> resource(new Resource);

        std::string texturePath =
                m_textureDirectory + std::string(filename) + m_textureFiletype;
        if (!resource->loadTextureFromFile(texturePath)) {
            std::cerr << "Error loading texture of spritesheet " << filename
                      << ".\n";
        }

        std::string metadataPath = m_metadataDirectory + std::string(filename) +
                                   m_metadataFiletype;
        if (!resource->loadMetadataFromFile(metadataPath)) {
            std::cerr << "Error loading metadata of spritesheet " << filename
                      << ".\n";
        }

        this->m_resourceHolder->insert(id, std::move(resource));
    }
};