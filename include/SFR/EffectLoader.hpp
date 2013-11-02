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
	EffectLoader(Ptr<ResourceManager> notifier) : ResourceManager::Notifiee(notifier) {}
    void onEffectNew(Ptr<Effect> effect);
	void onShaderNew(Ptr<Shader> shader);
private:
    std::string fileContents(const std::string& path);
};

}
