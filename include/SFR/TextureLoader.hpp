/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"

namespace sfr {

/* Loads images using SFML */
class TextureLoader : public AssetTable::Listener {
public:
	TextureLoader(Ptr<AssetTable> notifier) : AssetTable::Listener(notifier) {}
    void onAsset(Ptr<Texture> texture);
};

}
