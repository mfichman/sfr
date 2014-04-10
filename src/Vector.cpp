/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Vector.hpp"

using namespace sfr;


Vector::Vector(Scalar x, Scalar y, Scalar z) : 
    x(x),
    y(y),
    z(z) {
}

Vector::Vector() :
    x(0),
    y(0),
    z(0) {
}

Scalar Vector::length() const {
    return sqrt(lengthSquared());
}

Scalar Vector::lengthSquared() const {
    return x*x + y*y + z*z;
}

Scalar Vector::distance(Vector const& other) const {
    return sqrt(distanceSquared(other));
}

Scalar Vector::distanceSquared(Vector const& other) const {
    return (other - (*this)).lengthSquared();
}

Scalar Vector::dot(Vector const& other) const {
    return x*other.x + y*other.y + z*other.z;
}

Vector Vector::lerp(Vector const& other, Scalar alpha) const {
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

Vector Vector::operator/(Scalar s) const {
    return Vector(x/s, y/s, z/s);
}

Vector Vector::operator*(Scalar s) const {
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
    Scalar norm = length();
    return Vector(x/norm, y/norm, z/norm);
}

Vector Vector::orthogonal() const {
    
    Vector ortho = cross(Vector(1.0f, 0.0f, 0.0f));
    if (ortho.lengthSquared() < 1e-8) {
        ortho = cross(Vector(0.0f, 1.0f, 0.0f));
    }
    
    return ortho.unit();    
}

GLvec3
Vector::vec3f() const {
    return GLvec3(x, y, z);
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
