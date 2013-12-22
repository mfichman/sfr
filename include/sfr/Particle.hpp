/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"
#include "sfr/Color.hpp"
#include "sfr/Node.hpp"

namespace sfr {

class Particle {
public:
    Vector position; // Position of the particle
    Vector velocity; // Velocity of the particle
    Color color; // Color blend of the particle
    float size; // Size of the particle in world coordinates
    float rotation; // Texture rotation about view z-axis
};


}
