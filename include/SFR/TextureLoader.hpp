/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/ResourceManager.hpp"

namespace SFR {

/* Loads images using SFML */
class TextureLoader : public ResourceManager::Notifiee {
public:
    void onTextureNew(Texture* texture);
};

}
