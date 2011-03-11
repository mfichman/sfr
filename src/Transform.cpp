/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Transform.hpp"
#include "SFR/Mesh.hpp"
#include <algorithm>

using namespace SFR;

const Matrix& Transform::transform() const {
    return transform_;
}

const Matrix& Transform::worldTransform() const {
    return worldTransform_;
}

Vector Transform::position() const {
    return transform_.origin();
}

Quaternion Transform::rotation() const {
    return transform_.rotation();
}

Vector Transform::worldPosition() const {
    return worldTransform_.origin();
}

Quaternion Transform::worldRotation() const {
    return worldTransform_.rotation();
}

Iterator<Node> Transform::children() const {
    return children_.elements();
}

const std::string& Transform::name() const {
    return name_;
}

void Transform::transformIs(const Matrix& transform) {
    if (transform_ == transform) {
        return;
    }
    transform_ = transform;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onTransform();
    }
}

void Transform::worldTransformIs(const Matrix& transform) {
    if (worldTransform_ == transform) {
        return;
    }
    worldTransform_ = transform;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onWorldTransform();
    }
}

void Transform::positionIs(const Vector& position) {
    transformIs(Matrix(transform_.rotation(), position));

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onPosition();
    }
}

void Transform::rotationIs(const Quaternion& rotation) {
    transformIs(Matrix(rotation, transform_.origin()));

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onRotation();
    }
}

void Transform::nameIs(const std::string& name) {
    if (name == name_) {
        return;
    }

    name_ = name;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onName();
    }
}

void Transform::childNew(Node* child) {
    children_.elementNew(child);

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onChildNew(child);
    }
}

void Transform::childDel(Node* child) {
    children_.elementDel(child);

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onChildDel(child);
    }
}

void Transform::notifieeNew(Notifiee* notifiee) {
    notifiee_.push_back(notifiee);
}

void Transform::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}


void Transform::operator()(Functor* functor) {
    functor->operator()(this);
}