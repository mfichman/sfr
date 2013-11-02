/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Matrix.hpp"

namespace SFR {

/* Shadow map renderer. */
class ShadowRenderer : public Node::Functor {
public:
    ShadowRenderer(Ptr<ResourceManager> manager);
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
