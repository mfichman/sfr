/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

std::ostream& operator<<(std::ostream& out, const sfr::Quaternion& quaternion);
std::istream& operator>>(std::istream& in, sfr::Quaternion& quaternion);

namespace sfr {

class Quaternion {
public:
    Quaternion(float w, float x, float y, float z);
    Quaternion(const Vector& axis, float angle);
    Quaternion(const Vector& x, const Vector& y, const Vector& z);
    Quaternion(const Matrix& matrix);
	Quaternion(float w, const Vector& vector);
    Quaternion();
    
    friend std::ostream& ::operator<<(std::ostream& out, const Quaternion& quaternion);
    friend std::istream& ::operator>>(std::istream& in, Quaternion& quaternion);

    float length() const;
    float lengthSquared() const;
    float dot(const Quaternion& other) const;
    Vector operator*(const Vector& other) const;
    Quaternion operator*(float s) const;
    Quaternion operator+(const Quaternion& other) const;
    Quaternion operator-(const Quaternion& other) const;
    Quaternion operator*(const Quaternion& other) const;
    Quaternion operator-() const;
    Quaternion slerp(const Quaternion& other, float alpha) const;
    Quaternion inverse() const;
    Quaternion unit() const;
    bool operator==(const Quaternion other) const;
    bool operator!=(const Quaternion& other) const;
	Quaternion& operator+=(const Quaternion& other);
	Quaternion& operator-=(const Quaternion& other);

    static Quaternion IDENTITY;

    float w;
    float x;
    float y;
    float z;
};

}
