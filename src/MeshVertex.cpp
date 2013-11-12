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
    if (position != other.position) {
        return position < other.position;
    }
    if (normal != other.normal) {
        return normal < other.normal;
    }
    return texCoord < other.texCoord;
}

bool MeshVertex::operator==(MeshVertex const& other) const {
    return position == other.position && texCoord == other.texCoord
        && normal == other.normal;
}

bool MeshVertex::operator!=(MeshVertex const& other) const {
    return !this->operator==(other);
}

MeshVertex MeshVertex::operator+(MeshVertex const& other) const {
    MeshVertex out;
    out.position = position + other.position;
    out.normal = normal + other.normal;
    out.tangent = tangent + other.tangent;
    out.texCoord = texCoord + other.texCoord;
    return out;
}

MeshVertex MeshVertex::operator*(float scale) const {
    MeshVertex out;
    out.position = position * scale;
    out.normal = normal * scale;
    out.tangent = tangent * scale;
    out.texCoord = texCoord * scale;
    return out;
}
