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
#include <vector>

namespace SFR {

/* Simple point light with attenuation */
class SpotLight : public Node {
public:
    class Notifiee;

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
    DepthRenderTarget* shadowMap() const;
    const Matrix& transform() const;

    void diffuseColorIs(const Color& diffuse);
    void specularColorIs(const Color& specular);
    void directionIs(const Vector& direction);
    void constantAttenuationIs(float atten);
    void linearAttenuationIs(float atten);
    void quadraticAttenuationIs(float atten);
    void spotCutoffIs(float cutoff);
    void spotPowerIs(float power);
    void shadowMapIs(DepthRenderTarget* target);
    void transformIs(const Matrix& matrix);
    void notifieeNew(Notifiee* notifiee);
    void notifieeDel(Notifiee* notifiee);

    void operator()(Functor* functor);

private:
    Color diffuseColor_;
    Color specularColor_;
    float constantAttenuation_;
    float linearAttenuation_;
    float quadraticAttenuation_;
    float spotCutoff_;
    float spotPower_;
    Vector direction_;
    std::vector<Notifiee*> notifiee_;
    Ptr<DepthRenderTarget> shadowMap_;
    Matrix transform_;
};

class SpotLight::Notifiee : public Interface {
public:
    virtual void onDiffuseColor() {}
    virtual void onSpecularColor() {}
    virtual void onDirection() {}
    virtual void onConstantAttenuation() {}
    virtual void onLinearAttenuation() {}
    virtual void onQuadraticAttenuation() {}
    virtual void onSpotCutoff() {}
    virtual void onSpotPower() {}
    virtual void onShadowMap() {}
    virtual void onTransform() {}
};

}
