/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

std::ostream& operator<<(std::ostream& out, const sfr::Vector& vector);
std::istream& operator>>(std::istream& in, sfr::Vector& vector);

namespace sfr {

/* 3-dimensional vector utility class */
class Vector {
public:

    Vector(float x, float y, float z);
    Vector();

    float length() const;
    float lengthSquared() const;
    float distance(const Vector& other) const;
    float distanceSquared(const Vector& other) const;
    float dot(const Vector& other) const;
    Vector cross(const Vector& other) const;
    Vector operator+(const Vector& other) const;
    Vector operator-(const Vector& other) const;
    Vector operator*(const Vector& other) const;
    Vector operator/(const Vector& other) const;
	Vector operator/(float s) const;
    Vector operator-() const;
    Vector operator*(float s) const;
    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector project(const Vector& other) const;
    Vector unit() const;
    Vector orthogonal() const;
    Vector lerp(const Vector& other, float alpha) const;
    operator const float*() const;
    operator float*();

    friend std::ostream& ::operator<<(std::ostream& out, const Vector& vector);
    friend std::istream& ::operator>>(std::istream& in, Vector& vector);

    bool operator<(const Vector& other) const;
    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;

    static Vector ZERO;
    static Vector UNIT_X;
    static Vector UNIT_Y;
    static Vector UNIT_Z;

    float x;
    float y;
    float z;
};

}
