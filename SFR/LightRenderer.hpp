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
    LightRenderer(ResourceManager* manager);
    void operator()(Transform* transform);
    void operator()(PointLight* light);
    void operator()(Effect* effect);
    void operator()(Camera* camera);
    void operator()(HemiLight* light);
    void operator()(SpotLight* light);
    void operator()(Mesh* mesh);
    void operator()(AttributeBuffer* buffer);
    void operator()(IndexBuffer* buffer);

private:
    Ptr<Mesh> unitSphere_;
    Ptr<Effect> effect_;
    Ptr<Effect> directionalLight_;
    Ptr<Effect> pointLight_;
    Ptr<Effect> hemiLight_;
    Ptr<Effect> spotLight_;

    Matrix modelTransform_;
    Matrix projectionTransform_;
    Matrix viewTransform_;

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

    GLint direction_;
    GLint cutoff_;

    GLint position_;

    GLint model_;
    GLint view_;
    GLint projection_;
    GLint unproject_;
};

}