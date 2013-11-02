/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/AssetTable.hpp"
#include "sfr/WavefrontLoader.hpp"
#include "sfr/TextureLoader.hpp"
#include "sfr/Mesh.hpp"
#include "sfr/Material.hpp"
#include "sfr/Texture.hpp"
#include "sfr/Transform.hpp"
#include "sfr/EffectLoader.hpp"
#include "sfr/Effect.hpp"
#include "sfr/Shader.hpp"
#include <algorithm>

using namespace sfr;

void AssetTable::listenerIs(Listener* listener) const {
    if (listener) {
        listener_.push_back(listener);
    }
}

void AssetTable::listenerDel(Listener* listener) const {
    std::vector<Listener*>::iterator i = std::find(listener_.begin(), listener_.end(), listener);
    listener_.erase(i);
}
