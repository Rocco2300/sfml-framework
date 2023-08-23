#pragma once

#include "ResourceHolder.hpp"

#include <iostream>

template <typename ResHolder>
class ResourceLoader {
protected:
    using Identifier = typename ResHolder::IdentifierType;
    using Resource = typename ResHolder::ResourceType;

    ResHolder* m_resourceHolder{};

public:
    ResourceLoader() = default;
    ResourceLoader(ResHolder& resourceHolder) : m_resourceHolder{&resourceHolder} {}

    void setResourceHolder(ResHolder& resourceHolder) { m_resourceHolder = resourceHolder; }

    virtual void load(Identifier id, std::string_view filename) = 0;
};