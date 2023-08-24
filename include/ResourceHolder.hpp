#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>

template <typename Identifier, typename Resource>
class ResourceHolder {
public:
    using IdentifierType = Identifier;
    using ResourceType = Resource;

private:
    std::unordered_map<Identifier, std::unique_ptr<Resource>> m_assetMap;

public:
    void insert(Identifier id, std::unique_ptr<Resource>&& resource) {
        if (m_assetMap.count(id)) {
            return;
        }

        insertResource(id, std::move(resource));
    }

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