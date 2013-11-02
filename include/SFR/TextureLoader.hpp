/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/AssetTable.hpp"

namespace SFR {

/* Loads images using SFML */
class TextureLoader : public AssetTable::Notifiee {
public:
	TextureLoader(Ptr<AssetTable> notifier) : AssetTable::Notifiee(notifier) {}
    void onAsset(Ptr<Texture> texture);
};

}
