/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Transform.hpp"

namespace sfr {

/* Holds the root node and global scene data. */
class World : public Interface {
public:
    World();
    Ptr<Transform> root() const;
    Ptr<Camera> camera() const;
    Ptr<Cubemap> skybox() const;

    void cameraIs(Ptr<Camera> camera);
    void skyboxIs(Ptr<Cubemap> cubemap);

private:
    Ptr<Transform> root_;
    Ptr<Camera> camera_;
    Ptr<Cubemap> skybox_;
};

}
