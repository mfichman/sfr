/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#define GLEW_STATIC

#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>


namespace SFR {

class Interface;
class Node;
class Transform;
class Mesh;
class Material;
class Texture;
class Light;
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
class MeshObject;

}

#include "SFR/Ptr.hpp"


#undef far
#undef near