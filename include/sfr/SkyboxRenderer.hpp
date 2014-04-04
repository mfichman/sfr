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

/* Sky box renderer. */
class SkyboxRenderer : public Renderer {
public:
    SkyboxRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<Scene> scene);

private:
    Ptr<Scene> scene_;
    Ptr<Program> program_;
    Ptr<Mesh> unitSphere_;

    GLint transform_;
};

}
