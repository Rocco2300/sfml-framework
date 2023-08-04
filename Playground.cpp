#include "ResourceManager.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>

int main() {
    sf::err().rdbuf(nullptr);
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");

//    ResourceManager* resManager = ResourceManager::getInstance();
//    resManager->loadTexture("C:/Users/grigo/Repos/sfml-framework/testing.png");
//    resManager->loadTextureDirectory("C:/Users/grigo/Pictures/sprites");
//
//    sf::Sprite sprite;
//    sprite.setTexture(resManager->getTexture("testing"));
//    sprite.setTextureRect(sf::IntRect(48, 0, 48, 48));
//    sprite.setScale(4.f, 4.f);

    std::ifstream textureFile("C:/Users/grigo/Repos/sfml-framework/output.tex", std::ios::in | std::ios::binary);

    uint64_t imageSectionSize{};
    uint64_t metadataSectionSize{};
    textureFile.read(reinterpret_cast<char*>(&imageSectionSize), sizeof(imageSectionSize));
    textureFile.read(reinterpret_cast<char*>(&metadataSectionSize), sizeof(metadataSectionSize));

    std::cout << "Image size: " << imageSectionSize << std::endl;
    std::cout << "Metadata size: " << metadataSectionSize << std::endl;

    char* imageBuffer = new char[imageSectionSize];
    char* metadataBuffer = new char[metadataSectionSize];
    textureFile.read(imageBuffer, imageSectionSize);
    textureFile.read(metadataBuffer, metadataSectionSize);

    sf::Texture texture;
    if (!texture.loadFromMemory(imageBuffer, imageSectionSize)) {
        std::cout << "Error in loading texture from embedded image." << std::endl;
    }

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(48, 0, 48, 48));
    sprite.setScale(4.f, 4.f);

    std::cout << "Metadata: " << metadataBuffer << std::endl;

    delete[] imageBuffer;
    delete[] metadataBuffer;

    while (window.isOpen()) {
        sf::Event event{};

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}