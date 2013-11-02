/*****************************************************************************
 * Simple, Fast Renderer (SFR)                                               *
 * CS249b                                                                    *
 * Matt Fichman                                                              *
 * February, 2011                                                            *
 *****************************************************************************/
#pragma once

#include "SFR/Common.hpp"
#include "SFR/Node.hpp"
#include "SFR/Color.hpp"
#include "SFR/Vector.hpp"
#include "SFR/Matrix.hpp"

namespace SFR {

/* Simple point light with attenuation */
class SpotLight : public Node {
public:
    SpotLight();
    const Color& diffuseColor() const;
    const Color& specularColor() const;
    const Vector& direction() const;
    float constantAttenuation() const;
    float linearAttenuation() const;
    float quadraticAttenuation() const;
    float spotCutoff() const;
    float spotPower() const;
    float radiusOfEffect() const;
    Ptr<DepthRenderTarget> shadowMap() const;
    const Matrix& transform() const;

    void diffuseColorIs(const Color& diffuse);
    void specularColorIs(const Color& specular);
    void directionIs(const Vector& direction);
    void constantAttenuationIs(float atten);
    void linearAttenuationIs(float atten);
    void quadraticAttenuationIs(float atten);
    void spotCutoffIs(float cutoff);
    void spotPowerIs(float power);
    void shadowMapIs(Ptr<DepthRenderTarget> target);
    void transformIs(const Matrix& matrix);

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
