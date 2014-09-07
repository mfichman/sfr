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
    green(0.0f),
    blue(0.0f),
    alpha(1.0f) {
}

Color::Color(Scalar red, Scalar green, Scalar blue, Scalar alpha) :
    red(red),
    green(green),
    blue(blue),
    alpha(alpha) {
}

GLvec4
Color::vec4f() const {
    return GLvec4(red, green, blue, alpha);
}
    
Color Color::operator*(Scalar scale) const {
    return Color(red*scale, green*scale, blue*scale, alpha*scale);
}

Color Color::operator+(Color const& other) const {
    return Color(red+other.red, blue+other.blue, green+other.green, alpha+other.alpha);
}

std::ostream& operator<<(std::ostream& out, Color const& color) {
    return out << color.red << " " << color.blue << " " 
        << color.green << " " << color.alpha;
}

std::istream& operator>>(std::istream& in, Color& color) {
    return in >> color.red >> color.green >> color.blue;
}
