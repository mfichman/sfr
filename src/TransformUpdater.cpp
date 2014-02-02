/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/TransformUpdater.hpp"
#include "sfr/Camera.hpp"
#include "sfr/Transform.hpp"
#include "sfr/PointLight.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/HemiLight.hpp"
#include "sfr/World.hpp"
#include "sfr/Model.hpp"
#include "sfr/Particles.hpp"
#include "sfr/Ribbon.hpp"
#include "sfr/Billboard.hpp"

using namespace sfr;

void TransformUpdater::operator()(Ptr<World> world) {
    world_ = world;
    world_->renderDescDelAll();
    operator()(world_->root());
}

void TransformUpdater::operator()(Ptr<Transform> transform) {
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
    for (Iterator<std::vector<Ptr<Node>>> node = transform->children(); node; node++) {
        node(std::static_pointer_cast<TransformUpdater>(shared_from_this()));
    }
    transform_ = previous;
}

void TransformUpdater::operator()(Ptr<Camera> camera) {
	// Why is the view transform the inverse?
    camera->viewTransformIs(transform_.inverse());
    camera->worldTransformIs(transform_);
}

void TransformUpdater::operator()(Ptr<Model> model) {
    world_->renderDescIs(RenderDesc(model, 0, transform_));
}

void TransformUpdater::operator()(Ptr<PointLight> light) {
    world_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<HemiLight> light) {
    world_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<SpotLight> light) {
    world_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Particles> particles) {
    world_->renderDescIs(RenderDesc(particles, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Ribbon> ribbon) {
    world_->renderDescIs(RenderDesc(ribbon, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Billboard> billboard) {
    world_->renderDescIs(RenderDesc(billboard, 0, transform_));
}

