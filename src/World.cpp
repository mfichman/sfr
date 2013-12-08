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
    root_.reset(new Transform("root"));
}

Ptr<Transform> World::root() const {
    return root_;
}

Ptr<Camera> World::camera() const {
    return camera_;
}

Ptr<Cubemap> World::skybox() const {
    return skybox_;
}

void World::cameraIs(Ptr<Camera> camera) {
    camera_ = camera;
}

void World::skyboxIs(Ptr<Cubemap> skybox) {
    skybox_ = skybox;
}

