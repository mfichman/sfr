/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Assets.hpp"
#include "sfr/Cubemap.hpp"
#include "sfr/Texture.hpp"
#include "sfr/TextureLoader.hpp"
#include <SFML/Graphics.hpp>

using namespace sfr;

void TextureLoader::onAsset(Ptr<Texture> texture) {

    sf::Image image;
    std::ifstream in(texture->name());
    if (in) {
        if (!image.loadFromFile(texture->name())) {
            throw ResourceException("couldn't load image: "+texture->name());
        }
    } else {
        Asset const* data = sfr::assetPtr(texture->name());
        if (!data) {
            throw ResourceException("couldn't load image: "+texture->name());
        }
        if (!image.loadFromMemory(data->data, data->len)) {
            throw ResourceException("couldn't load image: "+texture->name());
        }
    }

    texture->widthIs(image.getSize().x);
    texture->heightIs(image.getSize().y);
    texture->dataIs(image.getPixelsPtr());
}

void TextureLoader::onAsset(Ptr<Cubemap> cubemap) {

    sf::Image image;
    std::ifstream in(cubemap->name());
    if (in) {
        if (!image.loadFromFile(cubemap->name())) {
            throw ResourceException("couldn't load image: "+cubemap->name());
        }
    } else {
        Asset const* data = sfr::assetPtr(cubemap->name());
        if (!data) {
            throw ResourceException("couldn't load image: "+cubemap->name());
        }
        if (!image.loadFromMemory(data->data, data->len)) {
            throw ResourceException("couldn't load image: "+cubemap->name());
        }
    }

    cubemap->widthIs(image.getSize().x);
    cubemap->heightIs(image.getSize().y);
    cubemap->dataIs(image.getPixelsPtr());
}
