/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#define GLEW_STATIC

#ifdef _WIN32
#define SFR_USE_GLEW
#define NOMINMAX
#include <windows.h>
#include <GL/glew.h>
#endif
#ifdef __linux__
#define SFR_USE_GLEW
#include <GL/glew.h>
#endif

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


namespace SFR {

class ShadowRenderer;
class FlatRenderer;
class Interface;
class Node;
class Transform;
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
class MeshLoader;
class MaterialLoader;
class EffectLoader;
class Camera;
class ResourceManager;
class DepthRenderTarget;
class Model;
class World;

}

#include "SFR/Ptr.hpp"


#undef far
#undef near
