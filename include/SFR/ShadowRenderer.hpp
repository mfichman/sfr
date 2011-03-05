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
    ShadowRenderer(ResourceManager* manager);
    void operator()(World* world);
    void operator()(Transform* transform);
    void operator()(PointLight* light);
    void operator()(HemiLight* light);
    void operator()(SpotLight* light);

private:
    Matrix modelTransform_;
    Ptr<FlatRenderer> flatRenderer_;
};

}