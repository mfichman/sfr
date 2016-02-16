/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Vector.hpp"

using namespace sfr;

Camera::Camera() {
    id_ = 0;
    far_ = 1000.f;
    near_ = .1f;
    fieldOfView_ = 45.f;
    state_ = INACTIVE;
    type_ = PERSPECTIVE;
    left_ = 0;
    right_ = 0;
    top_ = 0;
    bottom_ = 0;
    viewportWidth_ = 0;
    viewportHeight_ = 0;
    status_ = DIRTY;
}

Camera::~Camera() {
    glDeleteBuffers(1, &id_);
}

Camera::Type Camera::type() const {
    return type_;
}

Camera::Status Camera::status() const {
    return status_; 
}

Scalar Camera::far() const {
    return far_;
}

Scalar Camera::near() const {
    return near_;
}

Scalar Camera::left() const {
    return left_;
}

Scalar Camera::right() const {
    return right_;
}

Scalar Camera::top() const {
    return top_;
}

Scalar Camera::bottom() const {
    return bottom_;
}

Scalar Camera::fieldOfView() const {
    return fieldOfView_;
}

GLuint Camera::viewportWidth() const {
    return viewportWidth_;
}

GLuint Camera::viewportHeight() const {
    return viewportHeight_;
}

Camera::State Camera::state() const {
    return state_;
}

Frustum Camera::viewFrustum() const {
    return viewFrustum(near_, far_);
}

Frustum Camera::viewFrustum(Scalar near, Scalar far) const {

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
        // Find the width and height of the near and far planes
        Scalar ratio = viewportWidth_/Scalar(viewportHeight_);
        Scalar tang = Scalar(tan(fieldOfView()*M_PI/360.f));
        Scalar nh = near * tang; // Height of the near plane
        Scalar nw = nh * ratio; // Width of the near plane
        Scalar fh = far * tang; // Height of the far plane
        Scalar fw = fh * ratio; // Width of the near plane

        // Get "look at" vectors.  The AT vector is simply a vector somewhere
        // along the local +z axis for the camera.
        Vector eye(0, 0, 0);//= matrix.origin();

        // Construct orthogonal basis.
        Vector x(1, 0, 0);//= matrix.right();
        Vector y(0, 1, 0);//= matrix.up();
        Vector z(0, 0, 1);//= -matrix.forward();

        // Compute the centers of the near and far planes
        Vector nc = eye - z * near;
        Vector fc = eye - z * far;

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

Matrix const& Camera::projectionMatrix() const {
    return buffer_.projectionMatrix_;
}

Matrix const& Camera::projectionMatrixInv() const {
    return buffer_.projectionMatrixInv_;
}

Matrix const& Camera::viewMatrix() const {
    return buffer_.viewMatrix_;
}

Matrix const& Camera::viewMatrixInv() const {
    return buffer_.viewMatrixInv_;
}

Matrix const& Camera::viewProjectionMatrix() const {
    return buffer_.viewProjectionMatrix_;
}

Matrix const& Camera::viewProjectionMatrixInv() const {
    return buffer_.viewProjectionMatrixInv_;
}

void Camera::viewportWidthIs(GLuint width) {
    status_ = DIRTY;
    viewportWidth_ = width;
}

void Camera::viewportHeightIs(GLuint height) {
    status_ = DIRTY;
    viewportHeight_ = height;
}

void Camera::farIs(Scalar distance) {
    status_ = DIRTY;
    far_ = distance;
}

void Camera::nearIs(Scalar distance) {
    status_ = DIRTY;
    near_ = distance;
}

void Camera::leftIs(Scalar distance) {
    status_ = DIRTY;
    left_ = distance;
}

void Camera::rightIs(Scalar distance) {
    status_ = DIRTY;
    right_ = distance;
}

void Camera::topIs(Scalar distance) {
    status_ = DIRTY;
    top_ = distance;
}

void Camera::bottomIs(Scalar distance) {
    status_ = DIRTY;
    bottom_ = distance;
}

void Camera::fieldOfViewIs(Scalar view) {
    status_ = DIRTY;
    fieldOfView_ = view;
}

void Camera::stateIs(State state) {
    state_ = state;
}

void Camera::typeIs(Type type) {
    status_ = DIRTY;
    type_ = type;
}

void Camera::viewMatrixIs(Matrix const& matrix) {
    status_ = DIRTY;
    buffer_.viewMatrix_ = matrix;
}

void Camera::statusIs(Status status) {
    if (status_ == status) {
        return;
    }
    status_ = status;
    if (status_ == SYNCED) {

        if (!id_) {
            glGenBuffers(1, &id_);
        }

        if (ORTHOGRAPHIC == type_) {
            buffer_.projectionMatrix_ = Matrix::ortho(left_, right_, bottom_, top_, near_, far_);
        } else if (PERSPECTIVE == type_) {
            Scalar aspectRatio = viewportWidth_/Scalar(viewportHeight_);
            buffer_.projectionMatrix_ = Matrix::perspective(fieldOfView_, aspectRatio, near_, far_);
        } else {
            assert(!"invalid camera mode");
        }
        buffer_.projectionMatrixInv_ = buffer_.projectionMatrix_.inverse();
        buffer_.viewMatrixInv_ = buffer_.viewMatrix_.inverse();
        buffer_.viewProjectionMatrix_ = buffer_.projectionMatrix_ * buffer_.viewMatrix_;
        buffer_.viewProjectionMatrixInv_ = buffer_.viewProjectionMatrix_.inverse();

        glBindBuffer(GL_UNIFORM_BUFFER, id_);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer_), &buffer_, GL_STATIC_DRAW);
    }
}

GLuint Camera::id() const {
    return id_;
}

void Camera::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Camera>(shared_from_this()));
}
