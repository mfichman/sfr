/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"

namespace SFR {

/* Manages object instances (can be repeated) */
class Instance : public Node {
public:
    Ptr<Node> root() const;
    void rootIs(Ptr<Transform> root);

    operator()(Ptr<Functor> functor);

private:
    Ptr<Node> root_;
};

}
