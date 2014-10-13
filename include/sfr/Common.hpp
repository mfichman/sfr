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
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define NOMINMAX
    #define SFR_USE_GLEW
    #include <windows.h>
    #include <GL/glew.h>
    #include <GL/GL.h>
    #undef ABSOLUTE
    #undef far
    #undef near
#endif

#ifdef __linux__
    #define SFR_USE_GLEW
    #include <GL/glew.h>
    #include <GL/GL.h>
#endif

#ifdef __APPLE__
    #include <OpenGL/GL3.h>
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <iostream>
#include <map>
#include <stdexcept>
#include <fstream>
#include <functional>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "sfr/Ptr.hpp"
#include "sfr/ResourceException.hpp"

namespace sfr {

#ifdef SFR_DOUBLE_PRECISION
typedef double Scalar;
#else
typedef float Scalar;
#endif
struct Asset;

class AlphaRenderer;
class AssetTable;
class AssetLoader;
class AttributeBuffer;
class Billboard;
class Billboards;
class BillboardProgram;
class BillboardRenderer;
class Camera;
class Color;
class Cubemap;
class Decal;
class Decals;
class DecalProgram;
class DecalRenderer;
class DeferredRenderer;
class DepthRenderTarget;
class Program;
class ProgramLoader;
class FlatRenderer;
class Font;
class FontLoader;
class FrameBuffer;
class Frustum;
class HemiLight;
class IndexBuffer;
class LightProgram;
class LightRenderer;
class Material;
class MaterialLoader;
class ModelRenderer;
class Matrix;
class Mesh;
class Model;
class ModelProgram;
class Node;
class Particle;
class ParticleProgram;
class ParticleRenderer;
class Particles;
class PointLight;
class Program;
class Quad;
class QuadProgram;
class QuadRenderer;
class Quaternion;
class Renderer;
class RenderTarget;
class Ribbon;
class RibbonProgram;
class RibbonRenderer;
class Shader;
class ShadowRenderer;
class SpotLight;
class SkyboxRenderer;
class StreamDrawBuffer;
class Text;
class TextProgram;
class TextRenderer;
class Texture;
class TextureLoader;
class Transform;
class TransparencyRenderer;
class Ui;
class UiRenderer;
class Vector;
class WavefrontLoader;
class Scene;


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
