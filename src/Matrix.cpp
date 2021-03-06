/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#include "sfr/Common.hpp"
#include "sfr/Frustum.hpp"
#include "sfr/Matrix.hpp"
#include "sfr/Quaternion.hpp"
#include "sfr/Vector.hpp"

using namespace sfr;


Matrix Matrix::fromForwardVector(Vector const& forward) {
    Vector const zaxis = forward.unit();
    Vector const xaxis = forward.orthogonal().cross(zaxis).unit();
    Vector const yaxis = zaxis.cross(xaxis).unit();

    return Matrix(
        xaxis.x, xaxis.y, xaxis.z, 0,
        yaxis.x, yaxis.y, yaxis.z, 0,
        zaxis.x, zaxis.y, zaxis.z, 0,
        0,       0,       0,       1
    );
}

Matrix Matrix::frustum(Scalar l, Scalar r, Scalar b, Scalar t, Scalar n, Scalar f) {
    return Matrix(
        2*n/(r-l),    0,            (r+l)/(r-l),    0,
        0,            2*n/(t-b),    (t+b)/(t-b),    0,
        0,            0,            -(f+n)/(f-n),    -2*f*n/(f-n),
        0,            0,            -1,                0);
}

Matrix Matrix::ortho(Scalar l, Scalar r, Scalar b, Scalar t, Scalar n, Scalar f) {
    return Matrix(
        2/(r-l),    0,            0,                -(r+l)/(r-l),
        0,            2/(t-b),    0,                -(t+b)/(t-b),
        0,            0,            -2/(f-n),        -(f+n)/(f-n),
        0,            0,            0,                1);
}

Matrix Matrix::perspective(Scalar fov, Scalar aspect, Scalar n, Scalar f) {
    Scalar top = Scalar(tan(fov*M_PI/360.0f) * n);
    Scalar bottom = -top;
    Scalar right = aspect * top;
    Scalar left = aspect * bottom;
    return Matrix::frustum(left, right, bottom, top, n, f);
}

Matrix Matrix::look(Vector const& pos, Vector const& at, Vector const& sky) {
    Vector forward = (pos - at).unit();
    Vector right = sky.unit().cross(forward);
    Vector up = forward.cross(right);

    return Matrix::translate(pos) * Matrix::rotate(right, up, forward);
  /*
    //INVERSE:
    Vector forward = (pos - at).unit();
    Vector right = sky.unit().cross(forward);
    Vector up = forward.cross(right);

    Matrix data;
    data[0] = right.x;
    data[1] = up.x;
    data[2] = forward.x;
    data[3] = 0;
    
    data[4] = right.y;
    data[5] = up.y;
    data[6] = forward.y;
    data[7] = 0;

    data[8] = right.z;
    data[9] = up.z;
    data[10] = forward.z;
    data[11] = 0;

    return data * Matrix::translate(-pos);*/

}

Matrix Matrix::look(Vector const& direction) {
    Vector forward = direction.unit();
    Vector up = forward.orthogonal();
    Vector right = up.cross(forward);

    return Matrix::rotate(right, up, forward);
}

Matrix Matrix::rotate(Vector const& x, Vector const& y, Vector const& z) {
    Matrix data;
    // Right
    data[0] = x.x;
    data[1] = x.y;
    data[2] = x.z;
    data[3] = 0.0f;
    
    // Up
    data[4] = y.x;
    data[5] = y.y;
    data[6] = y.z;
    data[7] = 0.0f;
    
    // Forward
    data[8] = z.x;
    data[9] = z.y;
    data[10] = z.z;
    data[11] = 0.0f;
    
    data[12] = 0.0f;
    data[13] = 0.0f;
    data[14] = 0.0f;
    data[15] = 1.0f;

    return data;
}

Matrix Matrix::rotate(Quaternion const& rotation) {
    Matrix data;

    // This routine is borrowed from Ogre 3D
    Scalar fTx  = 2.0f*rotation.x;
    Scalar fTy  = 2.0f*rotation.y;
    Scalar fTz  = 2.0f*rotation.z;
    Scalar fTwx = fTx*rotation.w;
    Scalar fTwy = fTy*rotation.w;
    Scalar fTwz = fTz*rotation.w;
    Scalar fTxx = fTx*rotation.x;
    Scalar fTxy = fTy*rotation.x;
    Scalar fTxz = fTz*rotation.x;
    Scalar fTyy = fTy*rotation.y;
    Scalar fTyz = fTz*rotation.y;
    Scalar fTzz = fTz*rotation.z;

    data[0] = 1.0f-(fTyy+fTzz);
    data[4] = fTxy-fTwz;
    data[8] = fTxz+fTwy;
    data[12] = 0.0f;
    
    data[1] = fTxy+fTwz;
    data[5] = 1.0f-(fTxx+fTzz);
    data[9] = fTyz-fTwx;
    data[13] = 0.0f;
    
    data[2] = fTxz-fTwy;
    data[6] = fTyz+fTwx;
    data[10] = 1.0f-(fTxx+fTyy);
    data[14] = 0.0f;
    
    return data;
}

Matrix Matrix::scale(Scalar sx, Scalar sy, Scalar sz) {
    return Matrix(
        sx,     0,      0,      0, 
        0,      sy,     0,      0,
        0,      0,      sz,     0, 
        0,      0,      0,      1);
}

Matrix Matrix::translate(Vector const& trans) {
    Matrix data;
    data[12] = trans.x;
    data[13] = trans.y;
    data[14] = trans.z;

    return data;
}

Matrix::Matrix(const Scalar data[16]) {
    std::copy(data, data+16, this->data);
}

Matrix::Matrix(Scalar m00, Scalar m01, Scalar m02, Scalar m03,
    Scalar m10, Scalar m11, Scalar m12, Scalar m13,
    Scalar m20, Scalar m21, Scalar m22, Scalar m23,
    Scalar m30, Scalar m31, Scalar m32, Scalar m33) {
    
    data[0] = m00; data[4] = m01; data[8] = m02; data[12] = m03;
    data[1] = m10; data[5] = m11; data[9] = m12; data[13] = m13;
    data[2] = m20; data[6] = m21; data[10] = m22; data[14] = m23;
    data[3] = m30; data[7] = m31; data[11] = m32; data[15] = m33;
}

Matrix::Matrix(Quaternion const& rotation, Vector const& trans) {
    // This routine is borrowed from Ogre 3D
    Scalar fTx  = 2.0f*rotation.x;
    Scalar fTy  = 2.0f*rotation.y;
    Scalar fTz  = 2.0f*rotation.z;
    Scalar fTwx = fTx*rotation.w;
    Scalar fTwy = fTy*rotation.w;
    Scalar fTwz = fTz*rotation.w;
    Scalar fTxx = fTx*rotation.x;
    Scalar fTxy = fTy*rotation.x;
    Scalar fTxz = fTz*rotation.x;
    Scalar fTyy = fTy*rotation.y;
    Scalar fTyz = fTz*rotation.y;
    Scalar fTzz = fTz*rotation.z;

    data[0] = 1.0f-(fTyy+fTzz);
    data[4] = fTxy-fTwz;
    data[8] = fTxz+fTwy;
    data[12] = trans.x;
    
    data[1] = fTxy+fTwz;
    data[5] = 1.0f-(fTxx+fTzz);
    data[9] = fTyz-fTwx;
    data[13] = trans.y;
    
    data[2] = fTxz-fTwy;
    data[6] = fTyz+fTwx;
    data[10] = 1.0f-(fTxx+fTyy);
    data[14] = trans.z;
    
    data[3] = 0.0f;
    data[7] = 0.0f;
    data[11] = 0.0f;
    data[15] = 1.0f;
}

Matrix::Matrix() {
    std::fill_n(data, 16, 0.0f);

    data[0] = 1.0f;
    data[5] = 1.0f;
    data[10] = 1.0f;
    data[15] = 1.0f;
}

Matrix Matrix::operator*(Matrix const& other) const {
    Matrix out;

    static const Matrix ident;

    if (memcmp((void*)&ident, (void*)this, sizeof(ident)) == 0) {
        // optimize the identity case
        return other;
    }
    
    /*
    0  1  2  3
    4  5  6  7
    8  9  10 11
    12 13 14 15
    */
    
    /*
    0  4  8  12
    1  5  9  13
    2  6  10 14
    3  7  11 15
    */
    
    out.data[0] = other.data[0]*data[0] + other.data[1]*data[4] + other.data[2]*data[8] + other.data[3]*data[12];
    out.data[1] = other.data[0]*data[1] + other.data[1]*data[5] + other.data[2]*data[9] + other.data[3]*data[13];
    out.data[2] = other.data[0]*data[2] + other.data[1]*data[6] + other.data[2]*data[10] + other.data[3]*data[14];
    out.data[3] = other.data[0]*data[3] + other.data[1]*data[7] + other.data[2]*data[11] + other.data[3]*data[15];
    
    out.data[4] = other.data[4]*data[0] + other.data[5]*data[4] + other.data[6]*data[8] + other.data[7]*data[12];
    out.data[5] = other.data[4]*data[1] + other.data[5]*data[5] + other.data[6]*data[9] + other.data[7]*data[13];
    out.data[6] = other.data[4]*data[2] + other.data[5]*data[6] + other.data[6]*data[10] + other.data[7]*data[14];
    out.data[7] = other.data[4]*data[3] + other.data[5]*data[7] + other.data[6]*data[11] + other.data[7]*data[15];
    
    out.data[8] = other.data[8]*data[0] + other.data[9]*data[4] + other.data[10]*data[8] + other.data[11]*data[12];
    out.data[9] = other.data[8]*data[1] + other.data[9]*data[5] + other.data[10]*data[9] + other.data[11]*data[13];
    out.data[10] = other.data[8]*data[2] + other.data[9]*data[6] + other.data[10]*data[10] + other.data[11]*data[14];
    out.data[11] = other.data[8]*data[3] + other.data[9]*data[7] + other.data[10]*data[11] + other.data[11]*data[15];
    
    out.data[12] = other.data[12]*data[0] + other.data[13]*data[4] + other.data[14]*data[8] + other.data[15]*data[12];
    out.data[13] = other.data[12]*data[1] + other.data[13]*data[5] + other.data[14]*data[9] + other.data[15]*data[13];
    out.data[14] = other.data[12]*data[2] + other.data[13]*data[6] + other.data[14]*data[10] + other.data[15]*data[14];
    out.data[15] = other.data[12]*data[3] + other.data[13]*data[7] + other.data[14]*data[11] + other.data[15]*data[15];

    return out;
}

Vector Matrix::normal(Vector const& v) const {
    Vector out;
    
    out.x = data[0]*v.x + data[4]*v.y + data[8]*v.z;
    out.y = data[1]*v.x + data[5]*v.y + data[9]*v.z;
    out.z = data[2]*v.x + data[6]*v.y + data[10]*v.z;
    
    return out;
}

Vector Matrix::operator*(Vector const& v) const {
    Vector out;
    
    Scalar invw = 1.0f / (data[3]*v.x + data[7]*v.y + data[11]*v.z + data[15]);
    
    out.x = (data[0]*v.x + data[4]*v.y + data[8]*v.z + data[12])*invw;
    out.y = (data[1]*v.x + data[5]*v.y + data[9]*v.z + data[13])*invw;
    out.z = (data[2]*v.x + data[6]*v.y + data[10]*v.z + data[14])*invw;
    
    return out;
}

Vector Matrix::rotate(Vector const& v) const {
    Vector out;
    
    //Scalar invw = 1.0f / (data[3]*v.x + data[7]*v.y + data[11]*v.z + data[15]);
    
    out.x = (data[0]*v.x + data[4]*v.y + data[8]*v.z);// + data[12])*invw;
    out.y = (data[1]*v.x + data[5]*v.y + data[9]*v.z);// + data[13])*invw;
    out.z = (data[2]*v.x + data[6]*v.y + data[10]*v.z);// + data[14])*invw;
    
    return out;
}

Frustum Matrix::operator*(Frustum const& f) const {
    Frustum out;
    
    out.nearTopLeft = (*this) * f.nearTopLeft;
    out.nearTopRight = (*this) * f.nearTopRight;
    out.nearBottomLeft = (*this) * f.nearBottomLeft;
    out.nearBottomRight = (*this) * f.nearBottomRight;
    
    out.farTopLeft = (*this) * f.farTopLeft;
    out.farTopRight = (*this) * f.farTopRight;
    out.farBottomLeft = (*this) * f.farBottomLeft;
    out.farBottomRight = (*this) * f.farBottomRight;
    
    return out;
}

Matrix Matrix::inverse() const {
    // This inversion routine is taken from Ogre3D, version 1.7.
    Matrix out;
    
    Scalar m00 = data[0], m01 = data[4], m02 = data[8], m03 = data[12];
    Scalar m10 = data[1], m11 = data[5], m12 = data[9], m13 = data[13];
    Scalar m20 = data[2], m21 = data[6], m22 = data[10], m23 = data[14];
    Scalar m30 = data[3], m31 = data[7], m32 = data[11], m33 = data[15];

    Scalar v0 = m20 * m31 - m21 * m30;
    Scalar v1 = m20 * m32 - m22 * m30;
    Scalar v2 = m20 * m33 - m23 * m30;
    Scalar v3 = m21 * m32 - m22 * m31;
    Scalar v4 = m21 * m33 - m23 * m31;
    Scalar v5 = m22 * m33 - m23 * m32;

    Scalar t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
    Scalar t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
    Scalar t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
    Scalar t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

    Scalar invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

    out[0] = t00 * invDet;
    out[1] = t10 * invDet;
    out[2] = t20 * invDet;
    out[3] = t30 * invDet;

    out[4] = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    out[5] = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    out[6] = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    out[7] = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;

    out[8] = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    out[9] = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    out[10] = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    out[11] = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;

    out[12] = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
    out[13] = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
    out[14] = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
    out[15] = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
    
    return out;
}

GLmat4 Matrix::mat4f() const { 
    GLmat4 out;
    out.data[0] = data[0];
    out.data[1] = data[1];
    out.data[2] = data[2];
    out.data[3] = data[3];

    out.data[4] = data[4];
    out.data[5] = data[5];
    out.data[6] = data[6];
    out.data[7] = data[7];

    out.data[8] = data[8];
    out.data[9] = data[9];
    out.data[10] = data[10];
    out.data[11] = data[11];

    out.data[12] = data[12];
    out.data[13] = data[13];
    out.data[14] = data[14];
    out.data[15] = data[15];

    return out;
}
    
Vector Matrix::forward() const {
    return Vector(data[8], data[9], data[10]);
}
    
Vector Matrix::right() const {
    return Vector(data[0], data[1], data[2]);
}

Vector Matrix::up() const {
    return Vector(data[4], data[5], data[6]);
}
    
Vector Matrix::origin() const {
    return Vector(data[12], data[13], data[14]);
}

Quaternion Matrix::rotation() const {
    return Quaternion(right(), up(), forward());
}

Matrix Matrix::transpose() const {
    Matrix out;

    out[0] = data[0];
    out[1] = data[4];
    out[2] = data[8];
    out[3] = data[12];

    out[4] = data[1];
    out[5] = data[5];
    out[6] = data[9];
    out[7] = data[13];

    out[8] = data[2];
    out[9] = data[6];
    out[10] = data[10];
    out[11] = data[14];

    out[12] = data[3];
    out[13] = data[7];
    out[14] = data[11];
    out[15] = data[15];

    return out;
}
