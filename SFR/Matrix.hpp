/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"

namespace SFR {

/* 4x4 transformation matrix value type. */
class Matrix {
public:
	
	static Matrix frustum(float l, float r, float b, float t, float n, float f);
	static Matrix ortho(float l, float r, float b, float t, float n, float f);
	static Matrix perspective(float fovy, float aspect, float near, float far);
    static Matrix scale(float sx, float sy, float sz);

    Matrix(const float data[16]);

    Matrix(float m00, float m01, float m02, float m03,
           float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33);
    
    Matrix(const Quaternion& rotation, const Vector& trans);
    Matrix(const Quaternion& rotation);
    Matrix(const Vector& trans);
    Matrix(const Vector& x, const Vector& y, const Vector& z);
    Matrix();

    Matrix inverse() const;
    Matrix operator*(const Matrix& other) const;
    Vector operator*(const Vector& other) const;
    Frustum operator*(const Frustum& other) const;
    Vector rotate(const Vector& other) const;

    operator const float*() const;
    operator float*();
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
    float data[16];
};
}