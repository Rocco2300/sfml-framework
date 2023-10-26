#pragma once

#include "ResourceLoader.hpp"

#include <SFML/Audio/AlResource.hpp>
#include <SFML/Window/GlResource.hpp>

/**
 * @brief Concept checking if passed resource is from sfml
 * @tparam Resource - resource to check
 * @return true if sfml resource, false otherwise
 */
template <class Resource>
concept SFMLResource = std::is_base_of<sf::GlResource, Resource>::value ||
                       std::is_base_of<sf::AlResource, Resource>::value;

/**
 * @brief Resource loader class specialized for sfml resources.
 * @tparam ResHolder - Resource holder with ResourceType of a SFML resource.
 */
template <typename ResHolder>
    requires SFMLResource<typename ResHolder::ResourceType>
class SFMLResourceLoader : public ResourceLoader<ResHolder> {
private:
    using Identifier = typename ResHolder::IdentifierType;
    using Resource = typename ResHolder::ResourceType;

    std::string m_directory{};
    std::string m_filetype{};

public:
    /**
     * @brief Default constructor
     */
    SFMLResourceLoader() = default;

    /**
     * @brief Construct the loader with ResourceHolder reference
     * @param resourceHolder - reference to resource holder
     * @param directory - directory to load resources from
     * @param filetype - file extension of resource files
     */
    SFMLResourceLoader(ResHolder& resourceHolder,
                       std::string_view directory = "",
                       std::string_view filetype = "png")
        : ResourceLoader<ResHolder>(resourceHolder),
          m_directory{directory},
          m_filetype{filetype} {}

    /**
     * @brief Setter for the directory from which files are loaded
     * @param directory - path to the directory
     */
    void setDirectory(std::string_view directory) { m_directory = directory; }

    /**
     * @brief Setter for the filetype of the resources
     * @param filetype - file extension (e.g. "png")
     */
    void setFiletype(std::string_view filetype) { m_filetype = filetype; }

    /**
     * @brief Load the resource at directory/filename.filetype
     * @see setDirectory
     * @see setFiletype
     * @param id - identifier to insert it under
     * @param filename - name of file to load
     * @throws std::runtime_error error from resource holder on failed insert
     * @see ResourceHolder#insert
     */
    void load(Identifier id, std::string_view filename) override {
        std::unique_ptr<Resource> resource(new Resource);

        auto path = m_directory + std::string(filename) + '.' + m_filetype;
        if (!resource->loadFromFile(path)) {
            std::cerr << "Error loading resource from " << path << '\n';
        }

        this->m_resourceHolder->insert(id, std::move(resource));
    }
};