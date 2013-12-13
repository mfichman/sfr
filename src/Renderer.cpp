/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"
#include "sfr/Iterator.hpp"
#include "sfr/Transform.hpp"

namespace sfr {

void Renderer::operator()(Ptr<Transform> transform) {
    // Cache a pointer to the transform node that is lowest on the tree, so
    // that we can retrieve the world transform easily.
    if (transform->renderMode() == Transform::INVISIBLE) { return; }
    Matrix previous = transform_;
    switch (transform->transformMode()) {
    case Transform::WORLD: 
        transform_ = transform->transform();
        break;
    case Transform::INHERIT:
        transform_ = transform_ * transform->transform();
        break;
    }
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(std::static_pointer_cast<Renderer>(shared_from_this()));
    }
    transform_ = previous;
}

Matrix const& Renderer::worldTransform() const { 
    return transform_;
}

}
