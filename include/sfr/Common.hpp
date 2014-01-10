/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/

#ifndef SFR_COMMON_HPP
#define SFR_COMMON_HPP

#define GLEW_STATIC
#ifdef _WIN32
#define NOMINMAX
#define SFR_USE_GLEW
#include <windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#undef far
#undef near
#endif
#ifdef __linux__
#define SFR_USE_GLEW
#include <GL/glew.h>
#include <GL/GL.h>
#endif
#ifdef __APPLE__
//#define SFR_USE_GLEW
//#include <GL/glew.h>
#include <OpenGL/GL3.h>
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <iostream>
#include <map>
#include <stdexcept>
#include <fstream>
#include <memory>

#include "sfr/Ptr.hpp"

namespace sfr {

#ifdef SFR_SINGLE_PRECISION
typedef float Scalar;
#else
typedef double Scalar;
#endif

class ShadowRenderer;
class FlatRenderer;
class DeferredRenderer;
class MaterialRenderer;
class LightRenderer;
class TransparencyRenderer;
class Node;
class Transform;
class Instance;
class Mesh;
class Material;
class Texture;
class PointLight;
class HemiLight;
class SpotLight;
class AttributeBuffer;
class IndexBuffer;
class Effect;
class Shader;
class Matrix;
class Quaternion;
class Vector;
class Frustum;
class TexCoord;
class Color;
class WavefrontLoader;
class MaterialLoader;
class EffectLoader;
class TextureLoader;
class Camera;
class AssetTable;
class DepthRenderTarget;
class DeferredRenderTarget;
class Model;
class World;
class Particles;
class Particle;
class Ribbon;
class Cubemap;
class Billboard;


class GLmat4 {
public:
    GLfloat data[16];
    operator GLfloat const*() const { return data; }
};

class GLvec4 {
public:
    GLvec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) : x(x), y(y), z(z), w(w) {}
    GLvec4() : x(0), y(0), z(0), w(0) {}
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat w;
    operator GLfloat const*() const { return &x; }
};

class GLvec3 {
public:
    GLvec3(GLfloat x, GLfloat y, GLfloat z) : x(x), y(y), z(z) {}
    GLvec3() : x(0), y(0), z(0) {}
    GLfloat x;
    GLfloat y;
    GLfloat z;
    operator GLfloat const*() const { return &x; }
};

class GLvec2 {
public:
    GLvec2(GLfloat u, GLfloat v) : u(u), v(v) {}
    GLvec2() : u(0), v(0) {}
    GLfloat u;
    GLfloat v;
    operator GLfloat const*() const { return &u; }
};


}

#endif
