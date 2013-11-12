/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

std::ostream& operator<<(std::ostream& out, sfr::Vector const& vector);
std::istream& operator>>(std::istream& in, sfr::Vector& vector);

namespace sfr {

/* 3-dimensional vector utility class */
class Vector {
public:

    Vector(float x, float y, float z);
    Vector();

    float length() const;
    float lengthSquared() const;
    float distance(Vector const& other) const;
    float distanceSquared(Vector const& other) const;
    float dot(Vector const& other) const;
    Vector cross(Vector const& other) const;
    Vector operator+(Vector const& other) const;
    Vector operator-(Vector const& other) const;
    Vector operator*(Vector const& other) const;
    Vector operator/(Vector const& other) const;
	Vector operator/(float s) const;
    Vector operator-() const;
    Vector operator*(float s) const;
    Vector& operator+=(Vector const& other);
    Vector& operator-=(Vector const& other);
    Vector project(Vector const& other) const;
    Vector unit() const;
    Vector orthogonal() const;
    Vector lerp(Vector const& other, float alpha) const;
    operator const float*() const;
    operator float*();

    friend std::ostream& ::operator<<(std::ostream& out, Vector const& vector);
    friend std::istream& ::operator>>(std::istream& in, Vector& vector);

    bool operator<(Vector const& other) const;
    bool operator==(Vector const& other) const;
    bool operator!=(Vector const& other) const;

    static Vector ZERO;
    static Vector UNIT_X;
    static Vector UNIT_Y;
    static Vector UNIT_Z;

    float x;
    float y;
    float z;
};

}
