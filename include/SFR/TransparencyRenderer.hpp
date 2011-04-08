/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Matrix.hpp"
#include "SFR/Node.hpp"

namespace SFR {

/* Renders transparent meshes with a flat shading technique. */
class TransparencyRenderer : public Node::Functor {
public:
    TransparencyRenderer(ResourceManager* manager);
    void operator()(World* world);
    void operator()(Transform* transform);
    void operator()(Model* model);
    void operator()(Mesh* mesh);
    void operator()(Material* material);
    void operator()(Effect* effect);
    void operator()(AttributeBuffer* buffer);
    void operator()(IndexBuffer* buffer);

private:
    Matrix transform_;
    Ptr<Effect> effect_;
    Ptr<Effect> transparencyEffect_;
    Ptr<World> world_;

    GLint attrib_;
    GLint diffuse_;
    GLint opacity_;
    GLint position_;

    GLint model_;
    GLint view_;
    GLint projection_;
};

}
