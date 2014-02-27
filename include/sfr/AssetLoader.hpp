/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once


#include "sfr/Common.hpp"

namespace sfr {

/* Convenience object that sets up all built-in asset loaders */
class AssetLoader : public Interface {
public:
    AssetLoader(Ptr<AssetTable> notifier);

private:
    Ptr<FontLoader> fontLoader_;
    Ptr<WavefrontLoader> meshLoader_;
    Ptr<ProgramLoader> programLoader_;
    Ptr<TextureLoader> textureLoader_;
};



}

