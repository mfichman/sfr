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

/* Base class for all renderers.  Keeps track of the world transform */
class Renderer : public Node::Functor {
public:
    void operator()(Ptr<Transform> transform);
    Matrix const& worldTransform() const; // Current world transform

protected:
    Matrix worldTransform_;
};

}
