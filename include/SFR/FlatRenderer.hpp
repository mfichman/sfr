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
    FlatRenderer(ResourceManager* manager);
    void operator()(World* world);
    void operator()(TransformNode* transform);
    void operator()(Model* object);
    void operator()(Mesh* mesh);
    void operator()(Effect* effect);
    void operator()(AttributeBuffer* buffer);
    void operator()(IndexBuffer* buffer);

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
