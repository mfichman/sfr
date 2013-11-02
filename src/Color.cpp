/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Color.hpp"

using namespace sfr;

Color::Color() :
    red(0.0f),
    blue(0.0f),
    green(0.0f),
    alpha(1.0f) {
}

Color::Color(float red, float blue, float green, float alpha) :
    red(red),
    blue(blue),
    green(green),
    alpha(alpha) {
}

Color::operator const float*() const {
    return &red;
}
    
//! Returns a pointer to the internal float data of this color
Color::operator float*() {
    return &red;
}


std::ostream& operator<<(std::ostream& out, const Color& color) {
    return out << color.red << " " << color.blue << " " 
        << color.green << " " << color.alpha;
}

std::istream& operator>>(std::istream& in, Color& color) {
    return in >> color.red >> color.blue >> color.green;
}
