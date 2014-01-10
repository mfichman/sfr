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

    Vector(Scalar x, Scalar y, Scalar z);
    Vector();

    Scalar length() const;
    Scalar lengthSquared() const;
    Scalar distance(Vector const& other) const;
    Scalar distanceSquared(Vector const& other) const;
    Scalar dot(Vector const& other) const;
    Vector cross(Vector const& other) const;
    Vector operator+(Vector const& other) const;
    Vector operator-(Vector const& other) const;
    Vector operator*(Vector const& other) const;
    Vector operator/(Vector const& other) const;
	Vector operator/(Scalar s) const;
    Vector operator-() const;
    Vector operator*(Scalar s) const;
    Vector& operator+=(Vector const& other);
    Vector& operator-=(Vector const& other);
    Vector project(Vector const& other) const;
    Vector unit() const;
    Vector orthogonal() const;
    Vector lerp(Vector const& other, Scalar alpha) const;
    GLvec3 vec3f() const;

    friend std::ostream& ::operator<<(std::ostream& out, Vector const& vector);
    friend std::istream& ::operator>>(std::istream& in, Vector& vector);

    bool operator<(Vector const& other) const;
    bool operator==(Vector const& other) const;
    bool operator!=(Vector const& other) const;

    static Vector ZERO;
    static Vector UNIT_X;
    static Vector UNIT_Y;
    static Vector UNIT_Z;

    Scalar x;
    Scalar y;
    Scalar z;
};

}
