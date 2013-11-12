/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Node.hpp"
#include "sfr/Matrix.hpp"

namespace sfr {

/* Simple renderer.  Passes vertex, texture, and material info to a shader. */
class MaterialRenderer : public Node::Functor {
public:
    MaterialRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<Model> object);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Material> material);
    void operator()(Ptr<Texture> texture);
    void operator()(Ptr<Effect> effect);

private:
    Matrix transform_;
    Ptr<Effect> modelEffect_;
    Ptr<World> world_;
    
    GLint attrib_;
    GLint diffuseMap_;
    GLint specularMap_;
    GLint normalMap_;
    GLint emissiveMap_;

    GLint ambient_;
    GLint diffuse_;
    GLint specular_;
    GLint shininess_;
    GLint emissive_;

    GLint model_;
    GLint view_;
    GLint projection_;
    GLint normalMatrix_;
};

}
