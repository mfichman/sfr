/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"
#include "sfr/TexCoord.hpp"

namespace sfr {

class MeshVertex {
public:
    bool operator<(const MeshVertex& other) const;
    bool operator==(const MeshVertex& other) const;
    bool operator!=(const MeshVertex& other) const;
    MeshVertex operator+(const MeshVertex& other) const;
    MeshVertex operator*(float scale) const;

    Vector position;
    Vector normal;
    Vector tangent;
    TexCoord texCoord;
};

}
