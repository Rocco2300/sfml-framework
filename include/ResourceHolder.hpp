#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

/**
 * @brief Generic container class for game resources like textures, sounds, etc.
 * @tparam Identifier - Identifier type
 * @tparam Resource - Resource type
 */
template <typename Identifier, typename Resource>
class ResourceHolder {
public:
    using IdentifierType = Identifier;
    using ResourceType = Resource;

private:
    std::unordered_map<Identifier, std::unique_ptr<Resource>> m_assetMap;

public:
    /**
     * @brief Insert a resource in the ResourceHolder if not already inserted
     * @param id - identifier to insert under
     * @param resource - resource to insert
     * @throw std::runtime_error insertion failure
     */
    void insert(Identifier id, std::unique_ptr<Resource>&& resource) {
        if (m_assetMap.count(id)) {
            return;
        }

        insertResource(id, std::move(resource));
    }

    /**
     * @brief Get resource stored at specific id.
     * @param id - id to get
     * @return the resource if found
     * @throw std::runtime_error resource not found
     */
    Resource& get(Identifier id) {
        auto it = m_assetMap.find(id);
        if (it == m_assetMap.end()) {
            std::stringstream errorMessage;
            errorMessage << "ResourceHolder::get - resource " << id
                         << " doesn't exist";
            throw std::runtime_error(errorMessage.str());
        }

        return *it->second;
    }

    /**
     * @see get
     */
    const Resource& get(Identifier id) const {
        auto it = m_assetMap.find(id);
        if (it == m_assetMap.end()) {
            std::stringstream errorMessage;
            errorMessage << "ResourceHolder::get - resource " << id
                         << " doesn't exist";
            throw std::runtime_error(errorMessage.str());
        }

        return *it->second;
    }

private:
    void insertResource(Identifier id, std::unique_ptr<Resource> resource) {
        auto inserted =
                m_assetMap.insert(std::make_pair(id, std::move(resource)));

        if (!inserted.second) {
            std::stringstream errorMessage;
            errorMessage << "ResourceHolder::insertResource - failed to insert "
                         << id;
            throw std::runtime_error(errorMessage.str());
        }
    }
};