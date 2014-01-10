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

/* Renders transparent billboards with an alpha-blend technique. */
class BillboardRenderer : public Renderer {
public:
    BillboardRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Billboard> billboard);

private:
    Ptr<Effect> effect_;
    Ptr<World> world_;
    Ptr<Mesh> quad_;

    GLint texture_;
    GLint transform_;
    GLint tint_;
};

}
