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
    GLvec3 position; // Position of the particle
    GLvec3 velocity; // Velocity of the particle
    GLvec4 color; // Color blend of the particle
    GLfloat size; // Size of the particle in world coordinates
    GLfloat rotation; // Texture rotation about view z-axis
};


}
