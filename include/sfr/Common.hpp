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
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "sfr/Ptr.hpp"

namespace sfr {

#ifdef SFR_DOUBLE_PRECISION
typedef double Scalar;
#else
typedef float Scalar;
#endif

class AssetTable;
class AttributeBuffer;
class Billboard;
class Camera;
class Color;
class Cubemap;
class DeferredRenderer;
class DeferredRenderTarget;
class DepthRenderTarget;
class Effect;
class EffectLoader;
class FlatRenderer;
class Font;
class FontLoader;
class Frustum;
class HemiLight;
class IndexBuffer;
class Instance;
class LightRenderer;
class Material;
class MaterialLoader;
class MaterialRenderer;
class Matrix;
class Mesh;
class Model;
class Node;
class Particle;
class Particles;
class PointLight;
class Quaternion;
class Ribbon;
class Shader;
class ShadowRenderer;
class SpotLight;
class TexCoord;
class Texture;
class TextureLoader;
class Transform;
class TransparencyRenderer;
class Vector;
class WavefrontLoader;
class World;


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
