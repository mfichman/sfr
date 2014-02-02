/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"

namespace sfr {

/* Deferred shading renderer (1st and 2nd pass). */
class DeferredRenderer : public Node::Functor {
public:
    DeferredRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);

private:
    Ptr<ModelRenderer> materialPass_;
    Ptr<LightRenderer> lightPass_;
    Ptr<TransparencyRenderer> transparencyPass_;
    Ptr<DeferredRenderTarget> renderTarget_;
};

}
