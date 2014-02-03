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

/* Loads GLSL vertex and fragment shader programs */
class ProgramLoader : public AssetTable::Listener {
public:
	ProgramLoader(Ptr<AssetTable> notifier) : AssetTable::Listener(notifier) {}
    void onAsset(Ptr<Program> program);
	void onAsset(Ptr<Shader> shader);
private:
    std::string fileContents(std::string const& path);
};

}
