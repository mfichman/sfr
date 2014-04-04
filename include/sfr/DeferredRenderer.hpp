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
    void operator()(Ptr<Scene> scene);

private:
    Ptr<ModelRenderer> materialPass_;
    Ptr<LightRenderer> lightPass_;
    Ptr<AlphaRenderer> alphaPass_;
    Ptr<ShadowRenderer> shadowPass_;
    Ptr<SkyboxRenderer> skyboxPass_;
    Ptr<UiRenderer> uiPass_;
    Ptr<DeferredRenderTarget> renderTarget_;
};

}
