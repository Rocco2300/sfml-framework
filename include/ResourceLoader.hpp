#pragma once

#include "ResourceHolder.hpp"

#include <iostream>

/**
 * @brief Common loader interface to inherit from.
 *
 * @def For easier time managing resources write your own loader for custom
 * resources. For reference on how you can write your own overload, see
 * the current child classes of it.
 *
 * @see SFMLResourceLoader
 * @see SpriteSheetLoader
 *
 * @tparam ResHolder
 */
template <typename ResHolder>
class ResourceLoader {
protected:
    using Identifier = typename ResHolder::IdentifierType;
    using Resource = typename ResHolder::ResourceType;

    ResHolder* m_resourceHolder{};

public:
    ResourceLoader() = default;
    ResourceLoader(ResHolder& resourceHolder)
        : m_resourceHolder{&resourceHolder} {}

    /**
     * @brief Setter for the reference of the resource holder in
     * which resources will be inserted on load
     * @param resourceHolder - reference to resource holder
     */
    void setResourceHolder(ResHolder& resourceHolder) {
        m_resourceHolder = resourceHolder;
    }

    /**
     * @brief Function to load a resource. Override in child classes.
     * @param id - resource identifier
     * @param filename - path to resource / name of file (see implementation)
     */
    virtual void load(Identifier id, std::string_view filename) = 0;
};