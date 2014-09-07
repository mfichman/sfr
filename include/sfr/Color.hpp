/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

std::ostream& operator<<(std::ostream& out, sfr::Color const& color);
std::istream& operator>>(std::istream& in, sfr::Color& color);

namespace sfr {

class Color {
public:
    Color(Scalar red, Scalar green, Scalar blue, Scalar alpha);
    Color();
    
    GLvec4 vec4f() const;
    Color operator*(Scalar scale) const;
    Color operator+(Color const& other) const;
    
    friend std::ostream& ::operator<<(std::ostream& out, Color const& color);
    friend std::istream& ::operator>>(std::istream& in, Color& color);

    Scalar red;
    Scalar green;
    Scalar blue;
    Scalar alpha;
};

}
