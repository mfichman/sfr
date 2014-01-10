/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "sfr/Common.hpp"
#include "sfr/Node.hpp"
#include "sfr/Color.hpp"
#include "sfr/Vector.hpp"
#include "sfr/Matrix.hpp"

namespace sfr {

/* Simple point light with attenuation */
class SpotLight : public Node {
public:
    SpotLight();
    Color const& diffuseColor() const;
    Color const& specularColor() const;
    Vector const& direction() const;
    Scalar constantAttenuation() const;
    Scalar linearAttenuation() const;
    Scalar quadraticAttenuation() const;
    Scalar spotCutoff() const;
    Scalar spotPower() const;
    Scalar radiusOfEffect() const;
    Ptr<DepthRenderTarget> shadowMap() const;
    Matrix const& transform() const;

    void diffuseColorIs(Color const& diffuse);
    void specularColorIs(Color const& specular);
    void directionIs(Vector const& direction);
    void constantAttenuationIs(Scalar atten);
    void linearAttenuationIs(Scalar atten);
    void quadraticAttenuationIs(Scalar atten);
    void spotCutoffIs(Scalar cutoff);
    void spotPowerIs(Scalar power);
    void shadowMapIs(Ptr<DepthRenderTarget> target);
    void transformIs(Matrix const& matrix);

    void operator()(Ptr<Functor> functor);

private:
    Color diffuseColor_;
    Color specularColor_;
    Scalar constantAttenuation_;
    Scalar linearAttenuation_;
    Scalar quadraticAttenuation_;
    Scalar spotCutoff_;
    Scalar spotPower_;
    Vector direction_;
    Ptr<DepthRenderTarget> shadowMap_;
    Matrix transform_;
};

}
