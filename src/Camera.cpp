/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#include "SFR/Camera.hpp"
#include "SFR/Common.hpp"
#include "SFR/Vector.hpp"
#include <cmath>

using namespace SFR;

#define RADIANS(x) ((x)*3.14f/180.0f)

Camera::Camera() {
    far_ = 1000.f;
    near_ = .1f;
    fieldOfView_ = 45.f;
    state_ = INACTIVE;
    type_ = PERSPECTIVE;
    left_ = 0;
    right_ = 0;
    top_ = 0;
    bottom_ = 0;
}

Camera::Type Camera::type() const {
    return type_;
}

float Camera::far() const {
    return far_;
}

float Camera::near() const {
    return near_;
}

float Camera::left() const {
    return left_;
}

float Camera::right() const {
    return right_;
}

float Camera::top() const {
    return top_;
}

float Camera::bottom() const {
    return bottom_;
}

float Camera::fieldOfView() const {
    return fieldOfView_;
}

Camera::State Camera::state() const {
    return state_;
}

Frustum Camera::viewFrustum() const {

    if (ORTHOGRAPHIC == type_) {
        Frustum frustum;
        frustum.nearTopLeft = Vector(left_, top_, near_);
        frustum.nearTopRight = Vector(right_, top_, near_);
        frustum.nearBottomLeft = Vector(left_, bottom_, near_);
        frustum.nearBottomRight = Vector(right_, bottom_, near_);
        
        frustum.farTopLeft = Vector(left_, top_, far_);
        frustum.farTopRight = Vector(right_, top_, far_);
        frustum.farBottomLeft = Vector(left_, bottom_, far_);
        frustum.farBottomRight = Vector(right_, bottom_, far_);

        return frustum;
    } else {

        // Ge the height, width, and shadow distance of the frustum
        GLfloat viewport[4];
        glGetFloatv(GL_VIEWPORT, viewport);

        // Find the width and height of the near and far planes
        float ratio = viewport[2]/viewport[3];
        float tang = tan(RADIANS(fieldOfView()) * 0.5f);
        float nh = near_ * tang; // Height of the near plane
        float nw = nh * ratio; // Width of the near plane
        float fh = far_ * tang; // Height of the far plane
        float fw = fh * ratio; // Width of the near plane

        // Get "look at" vectors.  The AT vector is simply a vector somewhere
        // along the local +z axis for the camera.
        Vector eye(0, 0, 0);//= matrix.origin();

        // Construct orthogonal basis.
        Vector z(0, 0, -1);//= -matrix.forward();
        Vector x(1, 0, 0);//= matrix.right();
        Vector y(0, 1, 0);//= matrix.up();

        // Compute the centers of the near and far planes
        Vector nc = eye - z * near_;
        Vector fc = eye - z * far_;

        // Compute the 4 corners of the frustum on the near plane
        Frustum frustum;
        frustum.nearTopLeft = nc + y * nh - x * nw;
        frustum.nearTopRight = nc + y * nh + x * nw;
        frustum.nearBottomLeft = nc - y * nh - x * nw;
        frustum.nearBottomRight = nc - y * nh + x * nw;

        // Compute the 4 corners of the frustum on the far plane
        frustum.farTopLeft = fc + y * fh - x * fw;
        frustum.farTopRight = fc + y * fh + x * fw;
        frustum.farBottomLeft = fc - y * fh - x * fw;
        frustum.farBottomRight = fc - y * fh + x * fw;

        return frustum;
    }
}

Matrix Camera::projectionTransform() const {

    if (ORTHOGRAPHIC == type_) {
        return Matrix::ortho(left_, right_, bottom_, top_, near_, far_);
    } else {
        GLfloat viewport[4];
        glGetFloatv(GL_VIEWPORT, viewport);
        float aspectRatio = viewport[2]/viewport[3];
        return Matrix::perspective(fieldOfView_, aspectRatio, near_, far_);
    }
}

const Matrix& Camera::viewTransform() const {
    return viewTransform_;
}

void Camera::farIs(float distance) {
    if (distance == far_) {
        return;
    }
    far_ = distance;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onFar();
    }
}

void Camera::nearIs(float distance) {
    if (distance == near_) {
        return;
    }
    near_ = distance;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onNear();
    }
}

void Camera::leftIs(float distance) {
    if (distance == left_) {
        return;
    }
    left_ = distance;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onLeft();
    }
}

void Camera::rightIs(float distance) {
    if (distance == right_) {
        return;
    }
    right_ = distance;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onRight();
    }
}

void Camera::topIs(float distance) {
    if (distance == top_) {
        return;
    }
    top_ = distance;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onTop();
    }
}

void Camera::bottomIs(float distance) {
    if (distance == bottom_) {
        return;
    }
    bottom_ = distance;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onBottom();
    }
}

void Camera::fieldOfViewIs(float view) {
    if (fieldOfView_ == view) {
        return;
    }
    fieldOfView_ = view;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onFieldOfView();
    }
}

void Camera::viewTransformIs(const Matrix& transform) {
    if (viewTransform_ == transform) {
        return;
    }
    viewTransform_ = transform;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onViewTransform();
    }
}

void Camera::stateIs(State state) {
    if (state_ == state) {
        return;
    }
    state_ = state;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onState();
    }
}

void Camera::typeIs(Type type) {
    if (type_ == type) {
        return;
    }
    type_ = type;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onType();
    }
}

void Camera::operator()(Functor* functor) {
    functor->operator()(this);
}