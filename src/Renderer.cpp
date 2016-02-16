/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Renderer.hpp"
#include "sfr/Transform.hpp"
#include "sfr/Scene.hpp"

namespace sfr {

void Renderer::operator()(Ptr<Scene> scene) {
    stateIs(ACTIVE);
    sceneIs(scene);
    for(Iterator<std::vector<RenderDesc>> i = scene->renderDescs(); i; i++) {
        worldMatrixIs(i->worldMatrix());
        i->node()->operator()(std::static_pointer_cast<Renderer>(shared_from_this()));
    }
    stateIs(Renderer::INACTIVE);
}

void Renderer::worldMatrixIs(Matrix const& transform) {
    worldMatrix_ = transform;
}

void Renderer::sceneIs(Ptr<Scene> scene) {
    scene_ = scene;
}

void Renderer::stateIs(State state) {
    if (state_ != state) {
        state_ = state;
        onState();
    }
}

Matrix const& Renderer::worldMatrix() const { 
    return worldMatrix_;
}

Ptr<Scene> Renderer::scene() const {
    return scene_;
}

Renderer::State Renderer::state() const {
    return state_;
}

}
