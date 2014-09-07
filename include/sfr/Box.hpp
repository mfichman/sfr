/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"

namespace sfr {

class Box {
public:
    Box() {};
    Box(Frustum const& frustum);

    void pointIs(Vector const& point);

    Vector max; // Maximum x/y/z coordinate
    Vector min; // Minimum x/y/z coordinate

    Vector center() const;
};

}
