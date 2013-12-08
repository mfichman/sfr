/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/TextureLoader.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Cubemap.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>

using namespace sfr;

void TextureLoader::onAsset(Ptr<Texture> texture) {

    sf::Image image;
    if (!image.loadFromFile(texture->name())) {
        throw std::runtime_error("Couldn't load image: " + texture->name());
    }

    texture->widthIs(image.getSize().x);
    texture->heightIs(image.getSize().y);
    texture->dataIs(image.getPixelsPtr());
}

void TextureLoader::onAsset(Ptr<Cubemap> cubemap) {

    sf::Image image;
    if (!image.loadFromFile(cubemap->name())) {
        throw std::runtime_error("Couldn't load image: " + cubemap->name());
    }

    cubemap->widthIs(image.getSize().x);
    cubemap->heightIs(image.getSize().y);
    cubemap->dataIs(image.getPixelsPtr());
}
