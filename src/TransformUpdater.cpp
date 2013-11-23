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

using namespace sfr;

void TransformUpdater::operator()(Ptr<World> world) {
    operator()(world->root());
}

void TransformUpdater::operator()(Ptr<Transform> transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(std::static_pointer_cast<TransformUpdater>(shared_from_this()));
    }
    transform->worldTransformIs(transform_);
    transform_ = previous;
}

void TransformUpdater::operator()(Ptr<Camera> camera) {
	// Why is the view transform the inverse?
    camera->viewTransformIs(transform_.inverse());
}
