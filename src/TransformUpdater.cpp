/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/TransformUpdater.hpp"
#include "SFR/Camera.hpp"
#include "SFR/TransformNode.hpp"
#include "SFR/PointLight.hpp"
#include "SFR/SpotLight.hpp"
#include "SFR/HemiLight.hpp"
#include "SFR/World.hpp"

using namespace SFR;

void TransformUpdater::operator()(World* world) {
    operator()(world->root());
}

void TransformUpdater::operator()(TransformNode* transform) {
    Matrix previous = transform_;
    transform_ = transform_ * transform->transform();
    for (Iterator<Node> node = transform->children(); node; node++) {
        node(this);
    }
    transform_ = previous;
}

void TransformUpdater::operator()(Camera* camera) {
    camera->viewTransformIs(transform_.inverse());
}
