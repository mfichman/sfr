/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/World.hpp"
#include "SFR/Camera.hpp"
#include <algorithm>

using namespace SFR;

World::World() {
    root_ = new TransformNode;
    camera_ = new SFR::Camera;
}

TransformNode* World::root() const {
    return root_.ptr();
}

Camera* World::camera() const {
    return camera_.ptr();
}

void World::rootIs(TransformNode* node) {
    if (root_ == node) {
        return;
    }
    root_ = node;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onRoot();
    }
}

void World::cameraIs(Camera* camera) {
    if (camera_ == camera) {
        return;
    }
    camera_ = camera;
    for (size_t i = 0; i < notifiee_.size(); i++) {
        notifiee_[i]->onCamera();
    }
}

void World::notifieeNew(Notifiee* notifiee) {
    if (notifiee) {
        notifiee_.push_back(notifiee);
    }
}

void World::notifieeDel(Notifiee* notifiee) {
    std::remove(notifiee_.begin(), notifiee_.end(), notifiee);
}
