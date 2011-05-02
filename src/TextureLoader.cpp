/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/TextureLoader.hpp"
#include "SFR/Texture.hpp"
#include <SFML/Graphics.hpp>
#include <stdexcept>

using namespace SFR;

void TextureLoader::onTextureNew(Texture* texture) {

    sf::Image image;
    if (!image.LoadFromFile(texture->name())) {
        throw std::runtime_error("Couldn't load image: " + texture->name());
    }

    texture->widthIs(image.GetWidth());
    texture->heightIs(image.GetHeight());
    texture->dataIs(image.GetPixelsPtr());
}