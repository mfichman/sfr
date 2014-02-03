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
    FlatRenderer(Ptr<AssetTable> manager, bool shadowPass=false);
    void operator()(Ptr<Model> object);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Transform> transform);

    using Renderer::operator();
private:
    void onState();
    Ptr<ModelProgram> program_;

    bool shadowPass_;
};

}
