/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Matrix.hpp"

namespace sfr {

/* Shadow map renderer. */
class FlatRenderer : public Node::Functor {
public:
    FlatRenderer(Ptr<AssetTable> manager);
    void operator()(Ptr<World> world);
    void operator()(Ptr<Transform> transform);
    void operator()(Ptr<Model> object);
    void operator()(Ptr<Mesh> mesh);
    void operator()(Ptr<Effect> effect);
    void operator()(Ptr<AttributeBuffer> buffer);
    void operator()(Ptr<IndexBuffer> buffer);

private:
    Matrix transform_;
    Ptr<World> world_;
    Ptr<Effect> effect_;
    Ptr<Effect> flatShader_;

    GLint attrib_;
    GLint position_;

    GLint model_;
    GLint view_;
    GLint projection_;
};

}
