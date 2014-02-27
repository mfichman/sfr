/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2014                                                           *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetLoader.hpp"
#include "sfr/FontLoader.hpp"
#include "sfr/ProgramLoader.hpp"
#include "sfr/TextureLoader.hpp"
#include "sfr/WavefrontLoader.hpp"

using namespace sfr;

AssetLoader::AssetLoader(Ptr<AssetTable> assets) {
    fontLoader_.reset(new FontLoader(assets));
    meshLoader_.reset(new WavefrontLoader(assets));
    programLoader_.reset(new ProgramLoader(assets));
    textureLoader_.reset(new TextureLoader(assets));
}


