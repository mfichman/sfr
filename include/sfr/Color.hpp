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
    Color(float red, float blue, float green, float alpha);
    Color();

    operator const float*() const;
    operator float*();

    Color operator*(float scale) const;
    Color operator+(Color const& other) const;
    
    friend std::ostream& ::operator<<(std::ostream& out, Color const& color);
    friend std::istream& ::operator>>(std::istream& in, Color& color);

    float red;
    float blue;
    float green;
    float alpha;
};

}
