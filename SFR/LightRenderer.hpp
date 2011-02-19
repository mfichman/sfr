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

/* Deferred shading renderer (2nd pass). */
class LightRenderer : public Node::Functor {
public:
    void operator()(Transform* transform);
    void operator()(Light* light);
    void operator()(Effect* effect);

private:
    Ptr<Effect> effect_;
    Ptr<Effect> directionalLight_;
    Ptr<Effect> pointLight_;
    Ptr<Effect> hemiLight_;
    Ptr<Effect> spotLight_;

    Matrix transform_;

    GLint shadowMap_;
    GLint diffuseBuffer_;
    GLint specularBuffer_;
    GLint normalBuffer_;
    GLint depthBuffer_;

    GLint diffuse_;
    GLint backDiffuse_;
    GLint specular_;

    GLint atten0_;
    GLint atten1_;
    GLint atten2_;

    GLint position_;
    GLint direction_;
    GLint cutoff_;

    GLint model_;
    GLint view_;
    GLint projection_;
    GLint unproject_;
};

}