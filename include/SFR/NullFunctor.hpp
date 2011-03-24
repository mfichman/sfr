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
    void operator()(World* world);
    void operator()(Camera* camera);
    void operator()(TransformNode* transform);
    void operator()(PointLight* light);
    void operator()(Model* object);
    void operator()(HemiLight* light);
    void operator()(SpotLight* light);
};


}
