/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Transform.hpp"
#include "SFR/Mesh.hpp"
#include <algorithm>

using namespace SFR;

const Matrix& Transform::transform() const {
    return transform_;
}

Vector Transform::position() const {
    return transform_.origin();
}

Quaternion Transform::rotation() const {
    return transform_.rotation();
}

Iterator<Node> Transform::children() {
    return Iterator<Node>(children_);
}

const std::string& Transform::name() const {
    return name_;
}

void Transform::transformIs(const Matrix& transform) {
    if (transform_ == transform) {
        return;
    }
    transform_ = transform;
}

void Transform::positionIs(const Vector& position) {
    transformIs(Matrix(transform_.rotation(), position));
}

void Transform::rotationIs(const Quaternion& rotation) {
    transformIs(Matrix(rotation, transform_.origin()));
}

void Transform::nameIs(const std::string& name) {
    if (name == name_) {
        return;
    }

    name_ = name;
}

void Transform::childNew(Ptr<Node> child) {
    children_.push_back(child);
}

void Transform::childDel(Ptr<Node> child) {
    std::remove(children_.begin(), children_.end(), child);
}

void Transform::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Transform>(shared_from_this()));
}
