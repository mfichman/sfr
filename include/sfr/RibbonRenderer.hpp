/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"
#include "sfr/Node.hpp"

namespace sfr {

/* Renders ribbons of triangles */
class RibbonRenderer : public Renderer {
public:
    RibbonRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Ribbon> ribbon);

private:
    Ptr<Effect> effect_;
    Ptr<World> world_;

    GLint texture_;
    GLint model_;
    GLint view_;
    GLint projection_;
};

}
