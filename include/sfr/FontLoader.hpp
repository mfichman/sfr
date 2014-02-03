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

/* Loads fonts using freetype2 */
class FontLoader : public AssetTable::Listener {
public:
	FontLoader(Ptr<AssetTable> notifier) : AssetTable::Listener(notifier) {}
    void onAsset(Ptr<Font> font);

private:
    void loadFace(FT_Face face, unsigned char ch);
};

}
