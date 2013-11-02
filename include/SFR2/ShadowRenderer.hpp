/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Matrix.hpp"

namespace sfr {

/* Shadow map renderer. */
class ShadowRenderer : public Node::Functor {
public:
    ShadowRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<PointLight> light);
    void operator()(Ptr<SpotLight> light);

private:
    Matrix transform_;
    Ptr<FlatRenderer> flatRenderer_;
    Ptr<World> world_;
};

}
