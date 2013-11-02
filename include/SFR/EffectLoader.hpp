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

/* Loads GLSL vertex and fragment shader programs */
class EffectLoader : public AssetTable::Notifiee {
public:
	EffectLoader(Ptr<AssetTable> notifier) : AssetTable::Notifiee(notifier) {}
    void onAsset(Ptr<Effect> effect);
	void onAsset(Ptr<Shader> shader);
private:
    std::string fileContents(const std::string& path);
};

}
