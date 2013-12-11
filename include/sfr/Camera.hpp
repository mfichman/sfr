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
    Matrix projectionTransform() const;
    Matrix const& viewTransform() const;
    Matrix const& worldTransform() const;
    float far() const;
    float near() const;
    float left() const;
    float right() const;
    float top() const;
    float bottom() const;
    float fieldOfView() const;
    State state() const;
    Type type() const;

    void farIs(float distance);
    void nearIs(float distance);
    void leftIs(float distance);
    void rightIs(float distance);
    void topIs(float distance);
    void bottomIs(float distance);
    void fieldOfViewIs(float view);
    void stateIs(State state);
    void typeIs(Type type);
    void viewTransformIs(Matrix const& transform);
    void worldTransformIs(Matrix const& transform);

    void operator()(Ptr<Functor> functor);

private:
    Matrix viewTransform_;
    Matrix worldTransform_;
    float far_;
    float near_;
    float left_;
    float right_;
    float top_;
    float bottom_;
    float fieldOfView_;
    State state_;
    Type type_;
};

}
