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


void NullFunctor::operator()(Ptr<World> world) {
    world->root()->operator()(shared_from_this());
}

void NullFunctor::operator()(Ptr<Camera> camera) {
}

void NullFunctor::operator()(Ptr<Transform> transform) {
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(shared_from_this());
    }
}

void NullFunctor::operator()(Ptr<PointLight> light) {
}

void NullFunctor::operator()(Ptr<Model> object) {
}

void NullFunctor::operator()(Ptr<HemiLight> light) {
}

void NullFunctor::operator()(Ptr<SpotLight> light) {
}
