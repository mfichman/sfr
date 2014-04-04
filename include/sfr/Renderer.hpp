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

/* Base class for all renderers.  Keeps track of the scene transform */
class Renderer : public Node::Functor {
public:
    enum State { ACTIVE, INACTIVE };

    Matrix const& worldTransform() const; // Current scene transform
    State state() const;
    Ptr<Scene> scene() const;

    void worldTransformIs(Matrix const& transform);
    void sceneIs(Ptr<Scene> scene);
    void stateIs(State state);

    void operator()(Ptr<Scene> scene);

    virtual void onState() {}

private:
    Matrix worldTransform_;
    State state_ = INACTIVE;
    Ptr<Scene> scene_;
};

}
