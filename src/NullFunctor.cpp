/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/NullFunctor.hpp"
#include "SFR/World.hpp"

using namespace SFR;


void NullFunctor::operator()(World* world) {
    world->root()->operator()(this);
}

void NullFunctor::operator()(Camera* camera) {
}

void NullFunctor::operator()(TransformNode* transform) {
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }
}

void NullFunctor::operator()(PointLight* light) {
}

void NullFunctor::operator()(Model* object) {
}

void NullFunctor::operator()(HemiLight* light) {
}

void NullFunctor::operator()(SpotLight* light) {
}
