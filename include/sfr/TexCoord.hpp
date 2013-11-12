/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

std::ostream& operator<<(std::ostream& out, sfr::TexCoord const& vector);
std::istream& operator>>(std::istream& in, sfr::TexCoord& vector);

namespace sfr {

class TexCoord {
public:
    TexCoord(float u, float v);
    TexCoord();

    friend std::ostream& ::operator<<(std::ostream& out, TexCoord const& tex);
    friend std::istream& ::operator>>(std::istream& in, TexCoord& tex);

    bool operator<(TexCoord const& other) const;
    bool operator==(TexCoord const& other) const;
    bool operator!=(TexCoord const& other) const;
    TexCoord operator+(TexCoord const& other) const;
    TexCoord operator*(float scale) const;
    operator const float*() const;
    operator float*();

    float u;
    float v;
};

}
