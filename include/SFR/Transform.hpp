/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Interface.hpp"
#include "SFR/Node.hpp"
#include "SFR/List.hpp"
#include "SFR/Quaternion.hpp"
#include "SFR/Matrix.hpp"
#include "SFR/Vector.hpp"
#include "SFR/Iterator.hpp"
#include "SFR/Matrix.hpp"
#include "SFR/Vector.hpp"
#include "SFR/Quaternion.hpp"
#include <vector>

namespace SFR {

/* Rotation, translation, scaling node with children. */
class Transform : public Node {
public:
    class Notifiee;

    const Matrix& transform() const;
    const Matrix& worldTransform() const;
    Vector position() const;
    Quaternion rotation() const;
    Vector worldPosition() const;
    Quaternion worldRotation() const;
    Iterator<Node> children() const;

    void transformIs(const Matrix& transform);
    void worldTransformIs(const Matrix& transform);
    void positionIs(const Vector& position);
    void rotationIs(const Quaternion& rotation);
    void childNew(Node* child);
    void childDel(Node* child);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    List<Node> children_;
    Matrix transform_;
    Matrix worldTransform_;
    std::vector<Notifiee*> notifiee_;
};

class Transform::Notifiee : public Interface {
public:
    virtual void onTransform() {}
    virtual void onWorldTransform() {}
    virtual void onPosition() {}
    virtual void onRotation() {}
    virtual void onChildNew(Node* child) {}
    virtual void onChildDel(Node* child) {}
};

}