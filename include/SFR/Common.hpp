/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once


#ifndef SFR_DEPS_INCLUDED
#define SFR_DEPS_INLUCDED

#define GLEW_STATIC
#ifdef _WIN32
#define SFR_USE_GLEW
#define NOMINMAX
#include <windows.h>
#include <GL/glew.h>
#include <GL/GL.h>
#endif
#ifdef __linux__
#define SFR_USE_GLEW
#include <GL/glew.h>
#include <GL/GL.h>
#endif
#ifdef __APPLE__
#include <OpenGL/GL.h>
#endif

#include "SFR/Ptr.hpp"

#endif

namespace SFR {

class ShadowRenderer;
class FlatRenderer;
class DeferredRenderer;
class MaterialRenderer;
class LightRenderer;
class TransparencyRenderer;
class Interface;
class Node;
class TransformNode;
class InstanceNode;
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
class ResourceManager;
class DepthRenderTarget;
class DeferredRenderTarget;
class Model;
class World;

}

#undef far
#undef near
