/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Frustum.hpp"
#include "SFR/Matrix.hpp"

namespace SFR {


/* Contains parameters for the view camera. */
class Camera : public Node {
public:
    class Notifiee;
    enum State { ACTIVE, INACTIVE };
    enum Type { PERSPECTIVE, ORTHOGRAPHIC };

    Camera();
    
    Frustum viewFrustum() const;
    Matrix projectionTransform() const;
    const Matrix& viewTransform() const;
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
    void viewTransformIs(const Matrix& transform);

    void operator()(Functor* functor);

private:
    Matrix viewTransform_;
    float far_;
    float near_;
    float left_;
    float right_;
    float top_;
    float bottom_;
    float fieldOfView_;
    State state_;
    Type type_;
    std::vector<Notifiee*> notifiee_;
};

class Camera::Notifiee : public Interface {
public:
    virtual void onFar() {}
    virtual void onNear() {}
    virtual void onLeft() {}
    virtual void onRight() {}
    virtual void onTop() {}
    virtual void onBottom() {}
    virtual void onFieldOfView() {}
    virtual void onState() {}
    virtual void onType() {}
    virtual void onViewTransform() {}
};

}
