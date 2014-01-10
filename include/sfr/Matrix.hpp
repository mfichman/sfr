/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"

namespace sfr {

/* 4x4 transformation matrix value type. */
class Matrix {
public:
	
	static Matrix frustum(Scalar l, Scalar r, Scalar b, Scalar t, Scalar n, Scalar f);
	static Matrix ortho(Scalar l, Scalar r, Scalar b, Scalar t, Scalar n, Scalar f);
	static Matrix perspective(Scalar fovy, Scalar aspect, Scalar near, Scalar far);
    static Matrix scale(Scalar sx, Scalar sy, Scalar sz);
    static Matrix rotate(Quaternion const& rotation);
    static Matrix rotate(Vector const& x, Vector const& y, Vector const& z);
    static Matrix look(Vector const& pos, Vector const& at, Vector const& up);
    static Matrix look(Vector const& direction);
    static Matrix translate(Vector const& trans);

    Matrix(const Scalar data[16]);
    Matrix(Scalar m00, Scalar m01, Scalar m02, Scalar m03,
           Scalar m10, Scalar m11, Scalar m12, Scalar m13,
           Scalar m20, Scalar m21, Scalar m22, Scalar m23,
           Scalar m30, Scalar m31, Scalar m32, Scalar m33);
    Matrix(Quaternion const& rotation, Vector const& trans);
    Matrix();

    GLmat4 mat4f() const;
    Matrix inverse() const;
    Matrix transpose() const;
    Matrix operator*(Matrix const& other) const;
    Vector operator*(Vector const& other) const;
    Frustum operator*(Frustum const& other) const;
    Vector rotate(Vector const& other) const;
    Vector normal(Vector const& other) const;

    Vector forward() const;
    Vector right() const;
    Vector up() const;
    Vector origin() const;
    Quaternion rotation() const;
    
    /* Matrix layout (column major, a la OpenGL):
     * 0  4  8  12
     * 1  5  9  13
     * 2  6  10 14
     * 3  7  11 15
    */
    Scalar operator[](size_t index) const { return data[index]; }
    Scalar& operator[](size_t index) { return data[index]; }
private:
    Scalar data[16];
};
}
