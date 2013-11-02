/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Transform.hpp"

namespace SFR {

/* Holds the root node and global scene data. */
class World : public Interface<World> {
public:
    World();
    Ptr<Transform> root() const;
    Ptr<Camera> camera() const;

    void cameraIs(Ptr<Camera> camera);

private:
    Ptr<Transform> root_;
    Ptr<Camera> camera_;
};

}
