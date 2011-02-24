/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/MaterialRenderer.hpp"
#include "SFR/LightRenderer.hpp"
#include "SFR/Node.hpp"
#include "SFR/Matrix.hpp"
#include "SFR/DeferredRenderTarget.hpp"

namespace SFR {

/* Deferred shading renderer (1st and 2nd pass). */
class DeferredRenderer : public Node::Functor {
public:
    DeferredRenderer(ResourceManager* manager);
    void operator()(Transform* transform);

private:
    Ptr<MaterialRenderer> materialPass_;
    Ptr<LightRenderer> lightPass_;
    Ptr<DeferredRenderTarget> renderTarget_;
};

}