#include "TextureResource.hpp"

sf::Texture& TextureResource::get() {
    return m_texture;
}

sf::Texture &TextureResource::operator*() {
    return m_texture;
}

sf::Texture *TextureResource::operator->() {
    return &m_texture;
}

bool TextureResource::loadFromFile(const std::string &filename) {
    return m_texture.loadFromFile(filename);
}