/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"

using namespace SFR;

Node* Node::next() const {
    return next_.ptr();

}

void Node::nextIs(Node* node) {
    next_ = node;
}