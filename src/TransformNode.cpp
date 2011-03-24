/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/TransformNode.hpp"
#include "SFR/Mesh.hpp"
#include <algorithm>

using namespace SFR;

const Matrix& TransformNode::transform() const {
    return transform_;
}

Vector TransformNode::position() const {
    return transform_.origin();
}

Quaternion TransformNode::rotation() const {
    return transform_.rotation();
}

Iterator<Node> TransformNode::children() {
    return Iterator<Node>(children_);
}

const std::string& TransformNode::name() const {
    return name_;
}

void TransformNode::transformIs(const Matrix& transform) {
    if (transform_ == transform) {
        return;
    }
    transform_ = transform;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onTransform();
    }
}

void TransformNode::positionIs(const Vector& position) {
    transformIs(Matrix(transform_.rotation(), position));

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onPosition();
    }
}

void TransformNode::rotationIs(const Quaternion& rotation) {
    transformIs(Matrix(rotation, transform_.origin()));

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onRotation();
    }
}

void TransformNode::nameIs(const std::string& name) {
    if (name == name_) {
        return;
    }

    name_ = name;

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onName();
    }
}

void TransformNode::childNew(Node* child) {
    children_.push_back(child);

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onChildNew(child);
    }
}

void TransformNode::childDel(Node* child) {
    std::remove(children_.begin(), children_.end(), child);

    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onChildDel(child);
    }
}

void TransformNode::notifieeNew(Notifiee* notifiee) {
    notifiee_.push_back(notifiee);
}

void TransformNode::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}


void TransformNode::operator()(Functor* functor) {
    functor->operator()(this);
}
