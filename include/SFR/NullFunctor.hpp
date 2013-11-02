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

/* Empty functor implementation for performance testing */
class NullFunctor : public Node::Functor {
public:
    void operator()(Ptr<World> world);
    void operator()(Ptr<Camera> camera);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<PointLight> light);
    void operator()(Ptr<Model> object);
    void operator()(Ptr<HemiLight> light);
    void operator()(Ptr<SpotLight> light);
};


}
