/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Frustum.hpp"
#include "sfr/Matrix.hpp"

namespace sfr {


/* Contains parameters for the view camera. */
class Camera : public Node {
public:
    enum State { ACTIVE, INACTIVE };
    enum Type { PERSPECTIVE, ORTHOGRAPHIC };

    Camera();
    
    Frustum viewFrustum() const;
    Matrix transform() const;
    Matrix projectionTransform() const;
    Matrix const& viewTransform() const;
    Matrix const& worldTransform() const;
    Scalar far() const;
    Scalar near() const;
    Scalar left() const;
    Scalar right() const;
    Scalar top() const;
    Scalar bottom() const;
    Scalar fieldOfView() const;
    State state() const;
    Type type() const;

    void farIs(Scalar distance);
    void nearIs(Scalar distance);
    void leftIs(Scalar distance);
    void rightIs(Scalar distance);
    void topIs(Scalar distance);
    void bottomIs(Scalar distance);
    void fieldOfViewIs(Scalar view);
    void stateIs(State state);
    void typeIs(Type type);
    void viewTransformIs(Matrix const& transform);
    void worldTransformIs(Matrix const& transform);

    void operator()(Ptr<Functor> functor);

private:
    Matrix viewTransform_;
    Matrix worldTransform_;
    Scalar far_;
    Scalar near_;
    Scalar left_;
    Scalar right_;
    Scalar top_;
    Scalar bottom_;
    Scalar fieldOfView_;
    State state_;
    Type type_;
};

}
