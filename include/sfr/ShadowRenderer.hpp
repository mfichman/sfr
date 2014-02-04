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

/* Shadow map renderer. */
class ShadowRenderer : public Renderer {
public:
    ShadowRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<PointLight> light);
    void operator()(Ptr<SpotLight> light);

    using Renderer::operator();

private:
    Ptr<FlatRenderer> flatRenderer_;
};

}
