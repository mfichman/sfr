/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"
#include <cmath>

using namespace sfr;


Vector::Vector(float x, float y, float z) : 
    x(x),
    y(y),
    z(z) {
}

Vector::Vector() :
    x(0),
    y(0),
    z(0) {
}

float Vector::length() const {
    return sqrt(lengthSquared());
}

float Vector::lengthSquared() const {
    return x*x + y*y + z*z;
}

float Vector::distance(Vector const& other) const {
    return sqrt(distanceSquared(other));
}

float Vector::distanceSquared(Vector const& other) const {
    return (other - (*this)).lengthSquared();
}

float Vector::dot(Vector const& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Vector Vector::lerp(Vector const& other, float alpha) const {
    return *this * (1-alpha) + other * (alpha);
}

Vector Vector::cross(Vector const& other) const {
    return Vector(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x);
}

Vector Vector::operator+(Vector const& other) const {
    return Vector(x+other.x, y+other.y, z+other.z);
}

Vector Vector::operator-(Vector const& other) const {
    return Vector(x-other.x, y-other.y, z-other.z);
}

Vector Vector::operator*(Vector const& other) const {
    return Vector(x*other.x, y*other.y, z*other.z);
}

Vector Vector::operator/(Vector const& other) const {
    return Vector(x/other.x, y/other.y, z/other.z);
}

Vector Vector::operator-() const {
    return Vector(-x, -y, -z);
}

Vector Vector::operator/(float s) const {
	return Vector(x/s, y/s, z/s);
}

Vector Vector::operator*(float s) const {
    return Vector(s*x, s*y, s*z);
}

Vector& Vector::operator+=(Vector const& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector& Vector::operator-=(Vector const& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}


Vector Vector::project(Vector const& other) const {
    Vector u = unit();
    return u * other.dot(u);
}

Vector Vector::unit() const {
    float norm = length();
    return Vector(x/norm, y/norm, z/norm);
}

Vector Vector::orthogonal() const {
    
    Vector ortho = cross(Vector(1.0f, 0.0f, 0.0f));
    if (ortho.lengthSquared() < 1e-8) {
        ortho = cross(Vector(0.0f, 1.0f, 0.0f));
    }
    
    return ortho.unit();    
}

Vector::operator const float*() const {
    return &x;
}
    
Vector::operator float*() {
    return &x;
}

bool Vector::operator<(Vector const& other) const {
    if (x != other.x) return x < other.x;
    if (y != other.y) return y < other.y;
    if (z != other.z) return z < other.z;
    return false;
}
    
bool Vector::operator==(Vector const& other) const {
    if (x != other.x) return false;
    if (y != other.y) return false;
    if (z != other.z) return false;
    return true;
}
    
bool Vector::operator!=(Vector const& other) const {
    return !this->operator==(other);
}


std::ostream& operator<<(std::ostream& out, Vector const& vector) {
    return out << vector.x << " " << vector.y << " " << vector.z;
}

std::istream& operator>>(std::istream& in, Vector& vector) {
    return in >> vector.x >> vector.y >> vector.z;
}
