/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

std::ostream& operator<<(std::ostream& out, sfr::Quaternion const& quaternion);
std::istream& operator>>(std::istream& in, sfr::Quaternion& quaternion);

namespace sfr {

class Quaternion {
public:
    Quaternion(float w, float x, float y, float z);
    Quaternion(Vector const& axis, float angle);
    Quaternion(Vector const& x, Vector const& y, Vector const& z);
    Quaternion(Matrix const& matrix);
	Quaternion(float w, Vector const& vector);
    Quaternion();
    
    friend std::ostream& ::operator<<(std::ostream& out, Quaternion const& quaternion);
    friend std::istream& ::operator>>(std::istream& in, Quaternion& quaternion);

    float length() const;
    float lengthSquared() const;
    float dot(Quaternion const& other) const;
    Vector operator*(Vector const& other) const;
    Quaternion operator*(float s) const;
    Quaternion operator+(Quaternion const& other) const;
    Quaternion operator-(Quaternion const& other) const;
    Quaternion operator*(Quaternion const& other) const;
    Quaternion operator-() const;
    Quaternion slerp(Quaternion const& other, float alpha) const;
    Quaternion inverse() const;
    Quaternion unit() const;
    bool operator==(const Quaternion other) const;
    bool operator!=(Quaternion const& other) const;
	Quaternion& operator+=(Quaternion const& other);
	Quaternion& operator-=(Quaternion const& other);

    static Quaternion IDENTITY;

    float w;
    float x;
    float y;
    float z;
};

}
