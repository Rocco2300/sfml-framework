#pragma once

#include "ResourceHolder.hpp"

#include <iostream>

template <typename ResHolder>
class ResourceLoader {
private:
    ResHolder* m_resourceHolder{};

    std::string m_directory{};
    std::string m_fileType{};

    using Identifier = typename ResHolder::IdentifierType;
    using Resource = typename ResHolder::ResourceType;

public:
    ResourceLoader() = default;
    ResourceLoader(ResHolder& resourceHolder, std::string_view directory = "", std::string_view fileType = "")
        : m_resourceHolder{&resourceHolder}, m_directory{directory}, m_fileType{fileType} {}

    void setResourceHolder(ResHolder& resourceHolder) { m_resourceHolder = resourceHolder; }
    void setDirectory(std::string_view directory) { m_directory = directory; }
    void setFileType(std::string_view fileType) { m_fileType = fileType; }

    void load(Identifier id, std::string_view filename) {
        std::unique_ptr<Resource> resource(new Resource);

        std::string path = m_directory + std::string(filename) + m_fileType;
        if (!resource->loadFromFile(path)) {
            std::cerr << "Error loading resource from " << path << '\n';
        }

        m_resourceHolder->insert(id, std::move(resource));
    }
};