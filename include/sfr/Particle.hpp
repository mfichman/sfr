/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"
#include "sfr/Node.hpp"

namespace sfr {

class Particle {
public:
    Vector position;
    Vector velocity; 
    float time; // Creation time
    float size; // Size 
    float growth; // Change in size per sec
    float rotation; // Rotation about view z-axis
    float alpha; // Starting alpha
};


}
