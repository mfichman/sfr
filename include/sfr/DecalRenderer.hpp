/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"

namespace sfr {

/* Renders transparent particles with an alpha-blend technique. */
class DecalRenderer : public Renderer {
public:
    DecalRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Decals> decals);

    using Renderer::operator();

private:
    void onState();
    Ptr<DecalProgram> program_;
    Ptr<Mesh> unitCube_;
};

}
