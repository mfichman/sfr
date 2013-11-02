/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/World.hpp"
#include "sfr/Camera.hpp"
#include <algorithm>

using namespace sfr;

World::World() {
    root_ = std::make_shared<Transform>("root");
}

Ptr<Transform> World::root() const {
    return root_;
}

Ptr<Camera> World::camera() const {
    return camera_;
}

void World::cameraIs(Ptr<Camera> camera) {
    if (camera_ == camera) {
        return;
    }
    camera_ = camera;
}

