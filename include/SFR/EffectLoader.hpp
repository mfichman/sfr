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

/* Loads GLSL vertex and fragment shader programs */
class EffectLoader : public ResourceManager::Notifiee {
public:
    void onEffectNew(Effect* effect);

private:
    std::string fileContents(const std::string& path);
};

}