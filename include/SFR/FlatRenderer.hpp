/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Matrix.hpp"

namespace SFR {

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
