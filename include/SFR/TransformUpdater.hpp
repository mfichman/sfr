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

/* Updates node transformations in the scene graph */
class TransformUpdater : public Node::Functor {
public:
    void operator()(Ptr<World> world);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<Camera> camera);

private:
    Matrix transform_;
};


}
