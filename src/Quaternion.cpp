/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Quaternion.hpp"
#include "sfr/Vector.hpp"

using namespace sfr;


Quaternion Quaternion::IDENTITY;

Quaternion::Quaternion(Scalar w, Vector const& vector) : 
    w(w),
    x(vector.x),
    y(vector.y),
    z(vector.z) {

}

Quaternion::Quaternion(Scalar w, Scalar x, Scalar y, Scalar z) : 
    w(w),
    x(x),
    y(y),
    z(z) {
}

Quaternion::Quaternion(Vector const& axis, Scalar angle) {
    Scalar halfAngle = 0.5f*angle;
    Scalar sin = sinf(halfAngle);
    w = cosf(halfAngle);
    x = sin*axis.x;
    y = sin*axis.y;
    z = sin*axis.z;
}

Quaternion::Quaternion(Vector const& xaxis, Vector const& yaxis, Vector const& zaxis) {
    Scalar kRot[3][3];
    kRot[0][0] = xaxis.x;
    kRot[1][0] = xaxis.y;
    kRot[2][0] = xaxis.z;
    
    kRot[0][1] = yaxis.x;
    kRot[1][1] = yaxis.y;
    kRot[2][1] = yaxis.z;
    
    kRot[0][2] = zaxis.x;
    kRot[1][2] = zaxis.y;
    kRot[2][2] = zaxis.z;

    Scalar fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
    Scalar fRoot;

    if (fTrace > 0.0) {
        // |w| > 1/2, may as well choose w > 1/2
        fRoot = sqrtf(fTrace + 1.0f);  // 2w
        w = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;  // 1/(4w)
        x = (kRot[2][1]-kRot[1][2])*fRoot;
        y = (kRot[0][2]-kRot[2][0])*fRoot;
        z = (kRot[1][0]-kRot[0][1])*fRoot;
    } else {
        // |w| <= 1/2
        static size_t s_iNext[3] = { 1, 2, 0 };
        size_t i = 0;
        if ( kRot[1][1] > kRot[0][0] )
            i = 1;
        if ( kRot[2][2] > kRot[i][i] )
            i = 2;
        size_t j = s_iNext[i];
        size_t k = s_iNext[j];

        fRoot = sqrtf(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0f);
        Scalar* apkQuat[3] = { &x, &y, &z };
        *apkQuat[i] = 0.5f*fRoot;
        fRoot = 0.5f/fRoot;
        w = (kRot[k][j]-kRot[j][k])*fRoot;
        *apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
        *apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
    }
}

Quaternion::Quaternion() :
    w(1),
    x(0),
    y(0),
    z(0) {
}

  
Scalar Quaternion::length() const {
    return sqrtf(lengthSquared());
}

Scalar Quaternion::lengthSquared() const {
    return w*w + x*x + y*y + z*z;
}

Quaternion Quaternion::operator+(Quaternion const& other) const {
    return Quaternion(w+other.w, x+other.x, y+other.y, z+other.z);
}

Quaternion Quaternion::operator-(Quaternion const& other) const {
    return Quaternion(w-other.w, x-other.x, y-other.y, z-other.z);
}

Quaternion Quaternion::operator*(Quaternion const& other) const {
    return Quaternion(
        w*other.w - x*other.x - y*other.y - z*other.z,
        w*other.x + x*other.w + y*other.z - z*other.y,
        w*other.y + y*other.w + z*other.x - x*other.z,
        w*other.z + z*other.w + x*other.y - y*other.x);
}

Quaternion Quaternion::inverse() const {
    Scalar norm = lengthSquared();
    if (norm > 0) {
        return Quaternion(w/norm, -x/norm, -y/norm, -z/norm);
    } else {
        return Quaternion(0, 0, 0, 0);
    }
}

Scalar Quaternion::dot(Quaternion const& other) const {
    return w*other.w + x*other.x + y*other.y + z*other.z;
}

Vector Quaternion::operator*(Vector const& v) const {
    // nVidia SDK implementation
    Vector uv, uuv;
    Vector qvec(x, y, z);
    uv = qvec.cross(v);
    uuv = qvec.cross(uv);
    uv = uv * (2.0f * w);
    uuv = uuv * 2.0f;
    return v + uv + uuv;
}

Quaternion Quaternion::operator*(Scalar s) const {
    return Quaternion(w*s, x*s, y*s, z*s);
}

Quaternion Quaternion::operator-() const {
    return Quaternion(-w, -x, -y, -z);
}

Quaternion Quaternion::unit() const {
    Scalar norm = length();
    return Quaternion(w/norm, x/norm, y/norm, z/norm);
}

Quaternion Quaternion::slerp(Quaternion const& other, Scalar alpha) const {
    Scalar cos = this->dot(other);
    Quaternion rkt;

    if (cos < 0) {
        cos = -cos;
        rkt = -other;
    } else {
        rkt = other;
    }

    if (abs(cos) < 1 - 1e-03) {
        Scalar sin = std::sqrt(1 - cos*cos);
        Scalar angle = std::atan2(sin, cos);
        Scalar inv_sin = 1/sin;
        Scalar coeff0 = std::sin((1-alpha) * angle) * inv_sin;
        Scalar coeff1 = std::sin(alpha * angle) * inv_sin;
        return (*this)*coeff0 + rkt*coeff1;
    } else {
        rkt = (*this)*(1-alpha) + rkt*alpha;
        return rkt.unit();
    }
}

bool Quaternion::operator==(const Quaternion other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Quaternion::operator!=(Quaternion const& other) const {
    return !operator==(other);
}  

Quaternion& Quaternion::operator+=(Quaternion const& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Quaternion& Quaternion::operator-=(Quaternion const& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    w -= other.w;
    return *this;
}

std::ostream& operator<<(std::ostream& out, Quaternion const& quaternion) {
    return out << quaternion.w << " " << quaternion.x << " " 
        << quaternion.y << " " << quaternion.z;
}

std::istream& operator>>(std::istream& in, Quaternion& quaternion) {
    return in >> quaternion.w >> quaternion.x 
        >> quaternion.y >> quaternion.z;
}
