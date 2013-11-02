/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Node.hpp"
#include "SFR/Matrix.hpp"

namespace SFR {

/* Simple renderer.  Passes vertex, texture, and material info to a shader. */
class MaterialRenderer : public Node::Functor {
public:
    MaterialRenderer(Ptr<ResourceManager> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<Model> object);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Material> material);
    void operator()(Ptr<Texture> texture);
    void operator()(Ptr<Effect> effect);
    void operator()(Ptr<AttributeBuffer> buffer);
    void operator()(Ptr<IndexBuffer> buffer);

private:
    Matrix transform_;
    Ptr<Effect> effect_;
    Ptr<Effect> modelEffect_;
    Ptr<World> world_;
    
    GLint attrib_;
    GLint diffuseMap_;
    GLint specularMap_;
    GLint normalMap_;

    GLint ambient_;
    GLint diffuse_;
    GLint specular_;
    GLint shininess_;

    GLint normal_;
    GLint position_;
    GLint texCoord_;
    GLint tangent_;

    GLint model_;
    GLint view_;
    GLint projection_;
    GLint normalMatrix_;
};

}
