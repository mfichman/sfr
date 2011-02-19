/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/TexCoord.hpp"

using namespace SFR;

TexCoord::TexCoord(float u, float v) : u(u), v(v) {
}

TexCoord::TexCoord() : u(0), v(0) {
}

bool TexCoord::operator<(const TexCoord& other) const {
    static float epsilon = 0.0000001f;
    if (abs(u - other.u) < epsilon) return u < other.u;
    if (abs(v - other.v) < epsilon) return v < other.v;
    return false;
}

bool TexCoord::operator==(const TexCoord& other) const {
    static float epsilon = 0.0000001f;
    if (abs(u - other.u) > epsilon) return false;
    if (abs(v - other.v) > epsilon) return false;
    return true;
}

bool TexCoord::operator!=(const TexCoord& other) const {
    return !this->operator==(other);
}

TexCoord TexCoord::operator+(const TexCoord& other) const {
    return TexCoord(u + other.u, v + other.v);
}

TexCoord TexCoord::operator*(float scale) const {
    return TexCoord(u * scale, v * scale);
}

TexCoord::operator const float*() const {
    return &u;
}

TexCoord::operator float*() {
    return &u;
}

std::ostream& ::operator<<(std::ostream& out, const TexCoord& tex) {
    return out << tex.u << " " << tex.v;
}

std::istream& ::operator>>(std::istream& in, TexCoord& tex) {
    return in >> tex.u >> tex.v;
}
