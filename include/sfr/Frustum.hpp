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

class Frustum {
public:
    Vector nearTopLeft;
    Vector nearTopRight;
    Vector nearBottomLeft;
    Vector nearBottomRight;
    
    Vector farTopLeft;
    Vector farTopRight;
    Vector farBottomLeft;
    Vector farBottomRight;
};


}
