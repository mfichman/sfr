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
#include "sfr/World.hpp"

namespace sfr {

void Renderer::operator()(Ptr<World> world) {
    stateIs(ACTIVE);
    worldIs(world);
    for(Iterator<std::set<RenderDesc>> i = world->renderDescs(); i; i++) {
        worldTransformIs(i->worldTransform());
        i->node()->operator()(std::static_pointer_cast<Renderer>(shared_from_this()));
    }
    stateIs(Renderer::INACTIVE);
}

void Renderer::worldTransformIs(Matrix const& transform) {
    worldTransform_ = transform;
}

void Renderer::worldIs(Ptr<World> world) {
    world_ = world;
}

void Renderer::stateIs(State state) {
    if (state_ != state) {
        state_ = state;
        onState();
    }
}

Matrix const& Renderer::worldTransform() const { 
    return worldTransform_;
}

Ptr<World> Renderer::world() const {
    return world_;
}

Renderer::State Renderer::state() const {
    return state_;
}

}
