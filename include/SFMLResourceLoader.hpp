#pragma once

#include "ResourceLoader.hpp"

template <typename ResHolder>
class SFMLResourceLoader : public ResourceLoader<ResHolder> {
private:
    using Identifier = typename ResHolder::IdentifierType;
    using Resource = typename ResHolder::ResourceType;

    std::string m_directory{};
    std::string m_filetype{};

public:
    SFMLResourceLoader() = default;
    SFMLResourceLoader(ResHolder& resourceHolder,
                       std::string_view directory = "",
                       std::string_view filetype = "")
        : ResourceLoader<ResHolder>(resourceHolder),
          m_directory{directory},
          m_filetype{filetype} {}

    void setDirectory(std::string_view directory) { m_directory = directory; }
    void setFiletype(std::string_view filetype) { m_filetype = filetype; }

    void load(Identifier id, std::string_view filename) override {
        std::unique_ptr<Resource> resource(new Resource);

        std::string path = m_directory + std::string(filename) + m_filetype;
        if (!resource->loadFromFile(path)) {
            std::cerr << "Error loading resource from " << path << '\n';
        }

        this->m_resourceHolder->insert(id, std::move(resource));
    }
};