/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/NullFunctor.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;


void NullFunctor::operator()(Ptr<Scene> scene) {
    scene->root()->operator()(std::static_pointer_cast<NullFunctor>(shared_from_this()));
}

void NullFunctor::operator()(Ptr<Camera> camera) {
}

void NullFunctor::operator()(Ptr<Transform> transform) {
    for (Iterator<std::vector<Ptr<Node>>> node = transform->children(); node; node++) {
        node(std::static_pointer_cast<NullFunctor>(shared_from_this()));
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
