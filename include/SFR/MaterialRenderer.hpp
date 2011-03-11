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
    MaterialRenderer(ResourceManager* manager);
    void operator()(World* world);
    void operator()(Transform* transform);
    void operator()(Model* object);
    void operator()(Mesh* mesh);
    void operator()(Material* material);
    void operator()(Texture* texture);
    void operator()(Effect* effect);
    void operator()(AttributeBuffer* buffer);
    void operator()(IndexBuffer* buffer);

private:
    Matrix modelTransform_;
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