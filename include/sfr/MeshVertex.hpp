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

class MeshVertex {
public:
    bool operator<(MeshVertex const& other) const;
    bool operator==(MeshVertex const& other) const;
    bool operator!=(MeshVertex const& other) const;
    MeshVertex operator+(MeshVertex const& other) const;
    MeshVertex operator*(Scalar scale) const;

    GLvec3 position;
    GLvec3 normal;
    GLvec3 tangent;
    GLvec2 texCoord;
};

}
