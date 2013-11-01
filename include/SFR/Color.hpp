/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"

std::ostream& operator<<(std::ostream& out, const SFR::Color& color);
std::istream& operator>>(std::istream& in, SFR::Color& color);

namespace SFR {

class Color {
public:
    Color(float red, float blue, float green, float alpha);
    Color();

    operator const float*() const;
    operator float*();
    
    friend std::ostream& ::operator<<(std::ostream& out, const Color& color);
    friend std::istream& ::operator>>(std::istream& in, Color& color);

    float red;
    float blue;
    float green;
    float alpha;
};

}
