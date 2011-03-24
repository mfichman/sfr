/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/List.hpp"
#include "SFR/Quaternion.hpp"
#include "SFR/Matrix.hpp"
#include "SFR/Vector.hpp"
#include "SFR/Iterator.hpp"
#include <vector>

namespace SFR {

/* Rotation, translation, scaling node with children. */
class TransformNode : public Node {
public:
    class Notifiee;

    const Matrix& transform() const;
    Vector position() const;
    Quaternion rotation() const;
    Iterator<Node> children();
    const std::string& name() const;

    void transformIs(const Matrix& transform);
    void positionIs(const Vector& position);
    void rotationIs(const Quaternion& rotation);
    void nameIs(const std::string& name);
    void childNew(Node* child);
    void childDel(Node* child);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    std::vector<Ptr<Node> > children_;
    Matrix transform_;
    std::vector<Notifiee*> notifiee_;
    std::string name_;
};

class TransformNode::Notifiee : public Interface {
public:
    virtual void onTransform() {}
    virtual void onPosition() {}
    virtual void onRotation() {}
    virtual void onName() {}
    virtual void onChildNew(Node* child) {}
    virtual void onChildDel(Node* child) {}
};

}
