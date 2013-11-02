/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Quaternion.hpp"
#include "SFR/Matrix.hpp"
#include "SFR/Vector.hpp"
#include "SFR/Iterator.hpp"

namespace SFR {

/* Rotation, translation, scaling node with children. */
class Transform : public Node {
public:
    const Matrix& transform() const;
    Vector position() const;
    Quaternion rotation() const;
    Iterator<Node> children();
    const std::string& name() const;

    void transformIs(const Matrix& transform);
    void positionIs(const Vector& position);
    void rotationIs(const Quaternion& rotation);
    void nameIs(const std::string& name);
    void childNew(Ptr<Node> child);
    void childDel(Ptr<Node> child);

    void operator()(Ptr<Functor> functor);

private:
    std::vector<Ptr<Node> > children_;
    Matrix transform_;
    std::string name_;
};

}
