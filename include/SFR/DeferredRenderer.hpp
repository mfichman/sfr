/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"

namespace SFR {

/* Deferred shading renderer (1st and 2nd pass). */
class DeferredRenderer : public Node::Functor {
public:
    DeferredRenderer(Ptr<ResourceManager> manager);
    void operator()(Ptr<World> world);

private:
    Ptr<MaterialRenderer> materialPass_;
    Ptr<LightRenderer> lightPass_;
    Ptr<TransparencyRenderer> transparencyPass_;
    Ptr<DeferredRenderTarget> renderTarget_;
};

}
