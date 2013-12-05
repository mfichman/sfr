/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Transform.hpp"
#include "sfr/Mesh.hpp"
#include <algorithm>

using namespace sfr;

Transform::Transform(std::string const& name) {
    name_ = name;
    shadowMode_ = SHADOWED;
}

Matrix const& Transform::worldTransform() const {
    return worldTransform_;
}

Matrix const& Transform::transform() const {
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

std::string const& Transform::name() const {
    return name_;
}

Transform::ShadowMode Transform::shadowMode() const {
    return shadowMode_;
}

void Transform::transformIs(Matrix const& transform) {
    transform_ = transform;
}

void Transform::worldTransformIs(Matrix const& transform) {
    worldTransform_ = transform;
}

void Transform::positionIs(Vector const& position) {
    transformIs(Matrix(transform_.rotation(), position));
}

void Transform::rotationIs(Quaternion const& rotation) {
    transformIs(Matrix(rotation, transform_.origin()));
}

void Transform::childIs(Ptr<Node> child) {
    children_.push_back(child);
}

void Transform::childDel(Ptr<Node> child) {
    std::remove(children_.begin(), children_.end(), child);
}

void Transform::shadowModeIs(ShadowMode mode) {
    shadowMode_ = mode;
}

void Transform::operator()(Ptr<Functor> functor) {
    functor->operator()(std::static_pointer_cast<Transform>(shared_from_this()));
}
