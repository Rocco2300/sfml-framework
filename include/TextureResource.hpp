#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Rect.hpp>

#include "Resource.hpp"

class TextureResource : public Resource {
private:
    sf::Texture m_texture;

public:
    sf::Texture& get();
    sf::Texture& operator*();
    sf::Texture* operator->();

    bool loadFromFile(const std::string& filename) override;
};