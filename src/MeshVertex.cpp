/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/MeshVertex.hpp"

using namespace sfr;

bool MeshVertex::operator<(MeshVertex const& other) const {
    if (position.x != other.position.x) {
        return position.x < other.position.x;
    }
    if (position.y != other.position.y) {
        return position.y < other.position.y;
    }
    if (position.z != other.position.z) {
        return position.z < other.position.z;
    }

    if (normal.x != other.normal.x) {
        return normal.x < other.normal.x;
    }
    if (normal.y != other.normal.y) {
        return normal.y < other.normal.y;
    }
    if (normal.z != other.normal.z) {
        return normal.z < other.normal.z;
    }

    if (texCoord.u != other.texCoord.u) {
        return texCoord.u < other.texCoord.u;
    }
    return texCoord.v < other.texCoord.v;
}

bool MeshVertex::operator==(MeshVertex const& other) const {
    return position.x == other.position.x 
        && position.y == other.position.y 
        && position.z == other.position.z 
        && texCoord.u == other.texCoord.u
        && texCoord.v == other.texCoord.v
        && normal.x == other.normal.x
        && normal.y == other.normal.y
        && normal.z == other.normal.z;
}

bool MeshVertex::operator!=(MeshVertex const& other) const {
    return !this->operator==(other);
}
