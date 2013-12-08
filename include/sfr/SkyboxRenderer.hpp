/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Renderer.hpp"

namespace sfr {

/* Sky box renderer. */
class SkyboxRenderer : public Renderer {
public:
    SkyboxRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);

private:
    Ptr<World> world_;
    Ptr<Effect> effect_;
    Ptr<Mesh> unitSphere_;

    GLint model_;
    GLint view_;
    GLint projection_;
};

}
