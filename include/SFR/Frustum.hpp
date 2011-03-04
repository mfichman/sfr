/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Vector.hpp"

namespace SFR {

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