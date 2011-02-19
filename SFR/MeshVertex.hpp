/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Vector.hpp"
#include "SFR/TexCoord.hpp"

namespace SFR {

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