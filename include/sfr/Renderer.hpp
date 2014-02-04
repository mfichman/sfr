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
    enum State { ACTIVE, INACTIVE };

    Matrix const& worldTransform() const; // Current world transform
    State state() const;
    Ptr<World> world() const;

    void worldTransformIs(Matrix const& transform);
    void worldIs(Ptr<World> world);
    void stateIs(State state);

    void operator()(Ptr<World> world);

    virtual void onState() {}

private:
    Matrix worldTransform_;
    State state_ = INACTIVE;
    Ptr<World> world_;
};

}
