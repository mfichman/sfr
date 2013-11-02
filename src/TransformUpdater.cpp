/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/TransformUpdater.hpp"
#include "SFR/Camera.hpp"
#include "SFR/Transform.hpp"
#include "SFR/PointLight.hpp"
#include "SFR/SpotLight.hpp"
#include "SFR/HemiLight.hpp"
#include "SFR/World.hpp"

using namespace SFR;

void TransformUpdater::operator()(Ptr<World> world) {
    operator()(world->root());
}

void TransformUpdater::operator()(Ptr<Transform> transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(shared_from_this());
    }
    transform_ = previous;
}

void TransformUpdater::operator()(Ptr<Camera> camera) {
	// Why is the view transform the inverse?
    camera->viewTransformIs(transform_.inverse());
}
