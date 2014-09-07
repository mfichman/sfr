/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/


#include "sfr/Common.hpp"
#include "sfr/Box.hpp"
#include "sfr/Frustum.hpp"

using namespace sfr;

Box::Box(Frustum const& frustum) {
    Scalar inf = std::numeric_limits<Scalar>::max();
    min = sfr::Vector(inf, inf, inf);
    max = sfr::Vector(-inf, -inf, -inf);

    pointIs(frustum.nearTopLeft);
    pointIs(frustum.nearTopRight);
    pointIs(frustum.nearBottomLeft);
    pointIs(frustum.nearBottomRight);

    pointIs(frustum.farTopLeft);
    pointIs(frustum.farTopRight);
    pointIs(frustum.farBottomLeft);
    pointIs(frustum.farBottomRight);

}

void Box::pointIs(Vector const& point) {
    if (point.x < min.x) {
        min.x = point.x;
    } else if (point.x > max.x) {
        max.x = point.x;
    }
    if (point.y < min.y) {
        min.y = point.y;
    } else if (point.y > max.y) {
        max.y = point.y;
    }
    if (point.z < min.z) {
        min.z = point.z;
    } else if (point.z > max.z) {
        max.z = point.z;
    }
}

Vector Box::center() const {
    Vector center;
    center.x = (max.x+min.x)/2.f;
    center.y = (max.y+min.y)/2.f;
    center.z = (max.z+min.z)/2.f;
    return center;
}
