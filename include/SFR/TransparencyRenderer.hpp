/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Matrix.hpp"
#include "sfr/Node.hpp"

namespace sfr {

/* Renders transparent meshes with a flat shading technique. */
class TransparencyRenderer : public Node::Functor {
public:
    TransparencyRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<Model> model);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Material> material);
    void operator()(Ptr<Effect> effect);
    void operator()(Ptr<AttributeBuffer> buffer);
    void operator()(Ptr<IndexBuffer> buffer);

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
