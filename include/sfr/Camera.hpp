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
    enum Status { SYNCED, DIRTY };
    enum Type { PERSPECTIVE, ORTHOGRAPHIC };

    Camera();
    ~Camera();
    
    Frustum viewFrustum(Scalar near, Scalar far) const;
    Frustum viewFrustum() const;
    Matrix const& projectionMatrix() const;
    Matrix const& projectionMatrixInv() const;
    Matrix const& viewMatrix() const;
    Matrix const& viewMatrixInv() const;
    Matrix const& viewProjectionMatrix() const;
    Matrix const& viewProjectionMatrixInv() const;
    Scalar far() const;
    Scalar near() const;
    Scalar left() const;
    Scalar right() const;
    Scalar top() const;
    Scalar bottom() const;
    Scalar fieldOfView() const;
    GLuint viewportWidth() const;
    GLuint viewportHeight() const;
    State state() const;
    Type type() const;
    Status status() const;
    GLuint id() const;

    void farIs(Scalar distance);
    void nearIs(Scalar distance);
    void leftIs(Scalar distance);
    void rightIs(Scalar distance);
    void topIs(Scalar distance);
    void bottomIs(Scalar distance);
    void fieldOfViewIs(Scalar view);
    void stateIs(State state);
    void typeIs(Type type);
    void statusIs(Status status);
    void viewportWidthIs(GLuint width);
    void viewportHeightIs(GLuint height);
    void viewMatrixIs(Matrix const& matrix);

    void operator()(Ptr<Functor> functor);

private:
    GLuint id_;
    Scalar far_;
    Scalar near_;
    Scalar left_;
    Scalar right_;
    Scalar top_;
    Scalar bottom_;
    Scalar fieldOfView_;
    GLuint viewportWidth_;
    GLuint viewportHeight_;
    State state_;
    Type type_;

    // Cached computed values
    struct Buffer {
        Matrix projectionMatrix_;
        Matrix projectionMatrixInv_;
        Matrix viewMatrix_;
        Matrix viewMatrixInv_; 
        Matrix viewProjectionMatrix_;
        Matrix viewProjectionMatrixInv_;
    };

    mutable Buffer buffer_;
    mutable Status status_;
};

}
