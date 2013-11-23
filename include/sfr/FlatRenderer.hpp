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

/* Shadow map renderer. */
class FlatRenderer : public Renderer {
public:
    FlatRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Model> object);
    void operator()(Ptr<Mesh> mesh);

private:
    Ptr<World> world_;
    Ptr<Effect> effect_;

    GLint model_;
    GLint view_;
    GLint projection_;
};

}
