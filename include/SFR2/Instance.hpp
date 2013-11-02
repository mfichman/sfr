/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"

namespace sfr {

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
