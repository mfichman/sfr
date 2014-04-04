/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Billboards.hpp"
#include "sfr/Camera.hpp"
#include "sfr/HemiLight.hpp"
#include "sfr/Model.hpp"
#include "sfr/Particles.hpp"
#include "sfr/PointLight.hpp"
#include "sfr/Quad.hpp"
#include "sfr/Ribbon.hpp"
#include "sfr/SpotLight.hpp"
#include "sfr/Text.hpp"
#include "sfr/Transform.hpp"
#include "sfr/TransformUpdater.hpp"
#include "sfr/Scene.hpp"

using namespace sfr;

void TransformUpdater::operator()(Ptr<Scene> scene) {
    scene_ = scene;
    scene_->renderDescDelAll();
    operator()(scene_->root());
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
    scene_->renderDescIs(RenderDesc(model, 0, transform_));
}

void TransformUpdater::operator()(Ptr<PointLight> light) {
    scene_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<HemiLight> light) {
    scene_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<SpotLight> light) {
    scene_->renderDescIs(RenderDesc(light, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Particles> particles) {
    scene_->renderDescIs(RenderDesc(particles, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Ribbon> ribbon) {
    scene_->renderDescIs(RenderDesc(ribbon, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Billboards> billboards) {
    scene_->renderDescIs(RenderDesc(billboards, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Quad> quad) {
    scene_->renderDescIs(RenderDesc(quad, 0, transform_));
}

void TransformUpdater::operator()(Ptr<Text> text) {
    scene_->renderDescIs(RenderDesc(text, 0, transform_));
}


