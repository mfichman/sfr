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
    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;
    float spotCutoff() const;
    float spotPower() const;
    float radiusOfEffect() const;
    Ptr<DepthRenderTarget> shadowMap() const;
    Matrix const& transform() const;

    void diffuseColorIs(Color const& diffuse);
    void specularColorIs(Color const& specular);
    void directionIs(Vector const& direction);
    void constantAttenuationIs(float atten);
    void linearAttenuationIs(float atten);
    void quadraticAttenuationIs(float atten);
    void spotCutoffIs(float cutoff);
    void spotPowerIs(float power);
    void shadowMapIs(Ptr<DepthRenderTarget> target);
    void transformIs(Matrix const& matrix);

    void operator()(Ptr<Functor> functor);

private:
    Color diffuseColor_;
    Color specularColor_;
    float constantAttenuation_;
    float linearAttenuation_;
    float quadraticAttenuation_;
    float spotCutoff_;
    float spotPower_;
    Vector direction_;
    Ptr<DepthRenderTarget> shadowMap_;
    Matrix transform_;
};

}
