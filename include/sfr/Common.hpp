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

}

#endif
