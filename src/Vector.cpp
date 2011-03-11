/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "SFR/Common.hpp"
#include "SFR/Vector.hpp"
#include <cmath>

using namespace SFR;


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

float Vector::distance(const Vector& other) const {
    return sqrt(distanceSquared(other));
}

float Vector::distanceSquared(const Vector& other) const {
    return (other - (*this)).lengthSquared();
}

float Vector::dot(const Vector& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Vector Vector::lerp(const Vector& other, float alpha) const {
    return *this * (1-alpha) + other * (alpha);
}

Vector Vector::cross(const Vector& other) const {
    return Vector(
        y*other.z - z*other.y,
        z*other.x - x*other.z,
        x*other.y - y*other.x);
}

Vector Vector::operator+(const Vector& other) const {
    return Vector(x+other.x, y+other.y, z+other.z);
}

Vector Vector::operator-(const Vector& other) const {
    return Vector(x-other.x, y-other.y, z-other.z);
}

Vector Vector::operator*(const Vector& other) const {
    return Vector(x*other.x, y*other.y, z*other.z);
}

Vector Vector::operator/(const Vector& other) const {
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

Vector& Vector::operator+=(const Vector& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector& Vector::operator-=(const Vector& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}


Vector Vector::project(const Vector& other) const {
    Vector u = unit();
    return u * other.dot(u);
}

Vector Vector::unit() const {
    float norm = length();
    return Vector(x/norm, y/norm, z/norm);
}

Vector Vector::orthogonal() const {
    
    Vector ortho = cross(Vector(1.0f, 0.0f, 0.0f));
    if (ortho.lengthSquared() < 1e-12) {
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

bool Vector::operator<(const Vector& other) const {
    static float epsilon = 0.0000001f;
    if (abs(x - other.x) > epsilon) return x < other.x;
    if (abs(y - other.y) > epsilon) return y < other.y;
    if (abs(z - other.z) > epsilon) return z < other.z;
    return false;
}
    
bool Vector::operator==(const Vector& other) const {
    static float epsilon = 0.0000001f;
    if (abs(x - other.x) > epsilon) return false;
    if (abs(y - other.y) > epsilon) return false;
    if (abs(z - other.z) > epsilon) return false;
    return true;
}
    
bool Vector::operator!=(const Vector& other) const {
    return !this->operator==(other);
}


std::ostream& operator<<(std::ostream& out, const Vector& vector) {
    return out << vector.x << " " << vector.y << " " << vector.z;
}

std::istream& operator>>(std::istream& in, Vector& vector) {
    return in >> vector.x >> vector.y >> vector.z;
}